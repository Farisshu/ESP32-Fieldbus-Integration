/**
 * @file pid_control_module.c
 * @brief Self-contained PID Control Module dengan Filter dan FSM
 * 
 * Module ini mengimplementasikan:
 * - PID Controller (Position Form) dengan Anti-Windup
 * - Exponential Moving Average (EMA) Filter
 * - Finite State Machine (FSM) untuk Safety
 * 
 * Dirancang MISRA C:2012 compliant untuk safety-critical systems.
 * 
 * @author Embedded Control Systems Learning Module
 * @version 1.0.0
 * @date 2024
 * 
 * License: MIT (Educational Use)
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ========================================================================= */
/* TYPE DEFINITIONS                                                          */
/* ========================================================================= */

/** Floating-point type untuk komputasi kontrol */
typedef float ctrl_float_t;

/** Status sistem */
typedef enum {
    CTRL_STATE_INIT = 0,
    CTRL_STATE_IDLE = 1,
    CTRL_STATE_RUNNING = 2,
    CTRL_STATE_ESTOP = 3
} Control_State_t;

/** Struktur PID Controller */
typedef struct {
    /* Tuning parameters */
    ctrl_float_t kp;              /**< Proportional gain */
    ctrl_float_t ki;              /**< Integral gain (Ki * T) */
    ctrl_float_t kd;              /**< Derivative gain (Kd / T) */
    
    /* Limits */
    ctrl_float_t integral_limit;  /**< Maximum integral contribution */
    ctrl_float_t output_max;      /**< Maximum output value */
    ctrl_float_t output_min;      /**< Minimum output value */
    
    /* Internal state */
    ctrl_float_t integral_sum;    /**< Accumulated error */
    ctrl_float_t error_prev;      /**< Previous error */
    
    /* Status flags */
    bool initialized;             /**< Initialization flag */
    bool saturated;               /**< Output saturation flag */
} PID_Controller_t;

/** Struktur EMA Filter */
typedef struct {
    ctrl_float_t alpha;           /**< Smoothing factor (0 < α ≤ 1) */
    ctrl_float_t y_prev;          /**< Previous filtered output */
    bool initialized;             /**< Initialization flag */
} EMA_Filter_t;

/** Struktur System Context */
typedef struct {
    Control_State_t current_state;
    Control_State_t previous_state;
    bool estop_triggered;
    bool sensor_fault;
    uint32_t init_counter;
} System_Context_t;

/* ========================================================================= */
/* STATIC HELPER FUNCTIONS                                                   */
/* ========================================================================= */

/**
 * @brief Clamp value antara min dan max
 * @param value Nilai yang di- clamp
 * @param min_val Batas minimum
 * @param max_val Batas maximum
 * @return Nilai yang sudah di-clamp
 */
static ctrl_float_t clamp_value(ctrl_float_t value,
                                 ctrl_float_t min_val,
                                 ctrl_float_t max_val)
{
    ctrl_float_t result;
    
    if (value > max_val) {
        result = max_val;
    } else if (value < min_val) {
        result = min_val;
    } else {
        result = value;
    }
    
    return result;
}

/* ========================================================================= */
/* PID CONTROLLER IMPLEMENTATION                                             */
/* ========================================================================= */

/**
 * @brief Inisialisasi PID controller
 * @param pid Pointer ke struktur PID controller
 */
void PID_Init(PID_Controller_t *pid)
{
    if (pid == (PID_Controller_t *)0) {
        return;
    }
    
    /* Reset semua state variables */
    pid->integral_sum = 0.0F;
    pid->error_prev = 0.0F;
    pid->initialized = true;
    pid->saturated = false;
}

/**
 * @brief Update PID controller dengan setpoint dan measurement baru
 * @param pid Pointer ke struktur PID controller
 * @param setpoint Nilai yang diinginkan
 * @param measurement Nilai aktual dari sensor
 * @return Output kontroler (control signal)
 */
ctrl_float_t PID_Update(PID_Controller_t *pid,
                        ctrl_float_t setpoint,
                        ctrl_float_t measurement)
{
    ctrl_float_t error_curr;
    ctrl_float_t proportional_term;
    ctrl_float_t integral_term;
    ctrl_float_t derivative_term;
    ctrl_float_t control_output;
    
    /* Null pointer check */
    if (pid == (PID_Controller_t *)0) {
        return 0.0F;
    }
    
    /* Check initialization */
    if (pid->initialized != true) {
        return 0.0F;
    }
    
    /* Step 1: Calculate current error */
    error_curr = setpoint - measurement;
    
    /* Step 2: Proportional term */
    proportional_term = pid->kp * error_curr;
    
    /* Step 3: Integral term dengan anti-windup */
    pid->integral_sum = pid->integral_sum + error_curr;
    
    /* Clamp integral contribution */
    integral_term = pid->ki * pid->integral_sum;
    integral_term = clamp_value(integral_term,
                                 -(pid->integral_limit),
                                 pid->integral_limit);
    
    /* Anti-windup: reset integral sum jika clamped */
    if (integral_term >= pid->integral_limit) {
        pid->integral_sum = pid->integral_limit / pid->ki;
    } else if (integral_term <= -(pid->integral_limit)) {
        pid->integral_sum = -(pid->integral_limit) / pid->ki;
    }
    
    /* Step 4: Derivative term */
    derivative_term = pid->kd * (error_curr - pid->error_prev);
    
    /* Step 5: Sum all terms */
    control_output = proportional_term + integral_term + derivative_term;
    
    /* Step 6: Clamp final output */
    control_output = clamp_value(control_output,
                                  pid->output_min,
                                  pid->output_max);
    
    /* Update saturation flag */
    if ((control_output >= pid->output_max) ||
        (control_output <= pid->output_min)) {
        pid->saturated = true;
    } else {
        pid->saturated = false;
    }
    
    /* Step 7: Save state untuk next iteration */
    pid->error_prev = error_curr;
    
    return control_output;
}

/**
 * @brief Reset PID controller state
 * @param pid Pointer ke struktur PID controller
 */
void PID_Reset(PID_Controller_t *pid)
{
    if (pid == (PID_Controller_t *)0) {
        return;
    }
    
    pid->integral_sum = 0.0F;
    pid->error_prev = 0.0F;
    pid->saturated = false;
}

/* ========================================================================= */
/* EMA FILTER IMPLEMENTATION                                                 */
/* ========================================================================= */

/**
 * @brief Inisialisasi EMA filter
 * @param filter Pointer ke struktur EMA filter
 * @param alpha Smoothing factor (0 < α ≤ 1)
 */
void EMA_Init(EMA_Filter_t *filter, ctrl_float_t alpha)
{
    if (filter == (EMA_Filter_t *)0) {
        return;
    }
    
    /* Validate alpha range */
    if (alpha <= 0.0F) {
        alpha = 0.01F;
    } else if (alpha > 1.0F) {
        alpha = 1.0F;
    }
    
    filter->alpha = alpha;
    filter->y_prev = 0.0F;
    filter->initialized = true;
}

/**
 * @brief Update EMA filter dengan raw value baru
 * @param filter Pointer ke struktur EMA filter
 * @param raw_value Nilai mentah dari sensor (noisy)
 * @return Nilai yang sudah difilter
 */
ctrl_float_t EMA_Update(EMA_Filter_t *filter, ctrl_float_t raw_value)
{
    ctrl_float_t y_curr;
    
    if (filter == (EMA_Filter_t *)0) {
        return 0.0F;
    }
    
    if (filter->initialized != true) {
        return raw_value;
    }
    
    /* EMA equation: y[k] = α·x[k] + (1-α)·y[k-1] */
    y_curr = (filter->alpha * raw_value) +
             ((1.0F - filter->alpha) * filter->y_prev);
    
    filter->y_prev = y_curr;
    
    return y_curr;
}

/* ========================================================================= */
/* FINITE STATE MACHINE IMPLEMENTATION                                       */
/* ========================================================================= */

/**
 * @brief Inisialisasi system context
 * @param ctx Pointer ke struktur system context
 */
void SYS_Init(System_Context_t *ctx)
{
    if (ctx == (System_Context_t *)0) {
        return;
    }
    
    ctx->current_state = CTRL_STATE_INIT;
    ctx->previous_state = CTRL_STATE_INIT;
    ctx->estop_triggered = false;
    ctx->sensor_fault = false;
    ctx->init_counter = 0U;
}

/**
 * @brief Update FSM state berdasarkan input commands
 * @param ctx Pointer ke struktur system context
 * @param start_cmd Start command (true = start requested)
 * @param estop_cmd Emergency stop command (true = estop pressed)
 * @return Current state setelah update
 */
Control_State_t SYS_Update(System_Context_t *ctx,
                           bool start_cmd,
                           bool estop_cmd)
{
    uint_fast8_t i;
    Control_State_t next_state;
    
    /* Transition table untuk FSM */
    static const struct {
        Control_State_t from_state;
        bool start_cmd;
        bool estop_cmd;
        Control_State_t to_state;
    } state_transitions[] = {
        {CTRL_STATE_INIT,    false, false, CTRL_STATE_INIT},
        {CTRL_STATE_INIT,    true,  false, CTRL_STATE_INIT},
        {CTRL_STATE_INIT,    false, true,  CTRL_STATE_ESTOP},
        {CTRL_STATE_INIT,    true,  true,  CTRL_STATE_ESTOP},
        
        {CTRL_STATE_IDLE,    false, false, CTRL_STATE_IDLE},
        {CTRL_STATE_IDLE,    true,  false, CTRL_STATE_RUNNING},
        {CTRL_STATE_IDLE,    false, true,  CTRL_STATE_ESTOP},
        {CTRL_STATE_IDLE,    true,  true,  CTRL_STATE_ESTOP},
        
        {CTRL_STATE_RUNNING, false, false, CTRL_STATE_RUNNING},
        {CTRL_STATE_RUNNING, true,  false, CTRL_STATE_RUNNING},
        {CTRL_STATE_RUNNING, false, true,  CTRL_STATE_ESTOP},
        {CTRL_STATE_RUNNING, true,  true,  CTRL_STATE_ESTOP},
        
        {CTRL_STATE_ESTOP,   false, false, CTRL_STATE_ESTOP},
        {CTRL_STATE_ESTOP,   true,  false, CTRL_STATE_ESTOP},
        {CTRL_STATE_ESTOP,   false, true,  CTRL_STATE_ESTOP},
        {CTRL_STATE_ESTOP,   true,  true,  CTRL_STATE_ESTOP}
    };
    
    if (ctx == (System_Context_t *)0) {
        return CTRL_STATE_ESTOP;
    }
    
    ctx->previous_state = ctx->current_state;
    ctx->estop_triggered = estop_cmd;
    
    /* Special handling untuk INIT state */
    if (ctx->current_state == CTRL_STATE_INIT) {
        ctx->init_counter = ctx->init_counter + 1U;
        
        /* Simulasi initialization delay (contoh: 100 iterations) */
        if (ctx->init_counter >= 100U) {
            ctx->current_state = CTRL_STATE_IDLE;
            return ctx->current_state;
        }
        
        /* Check estop selama init */
        if (estop_cmd == true) {
            ctx->current_state = CTRL_STATE_ESTOP;
            return ctx->current_state;
        }
        
        return ctx->current_state;
    }
    
    /* State transition lookup */
    next_state = ctx->current_state;
    
    for (i = 0U; i < 16U; i++) {
        if (state_transitions[i].from_state == ctx->current_state) {
            if ((state_transitions[i].start_cmd == start_cmd) &&
                (state_transitions[i].estop_cmd == estop_cmd)) {
                next_state = state_transitions[i].to_state;
                break;
            }
        }
    }
    
    /* Stay in ESTOP sampai explicit reset */
    if ((ctx->current_state == CTRL_STATE_ESTOP) &&
        (estop_cmd == false) &&
        (start_cmd == false)) {
        next_state = CTRL_STATE_ESTOP;
    }
    
    ctx->current_state = next_state;
    return ctx->current_state;
}

/**
 * @brief Dapatkan nama state dalam format string
 * @param state State saat ini
 * @return String nama state
 */
const char* SYS_GetStateName(Control_State_t state)
{
    switch (state) {
        case CTRL_STATE_INIT:
            return "INIT";
        case CTRL_STATE_IDLE:
            return "IDLE";
        case CTRL_STATE_RUNNING:
            return "RUNNING";
        case CTRL_STATE_ESTOP:
            return "ESTOP";
        default:
            return "UNKNOWN";
    }
}

/* ========================================================================= */
/* EXAMPLE INTEGRATION (PSEUDOCODE)                                          */
/* ========================================================================= */

#if 0  /* Conditional compilation: tidak dikompilasi secara default */

/**
 * @brief Contoh integrasi lengkap untuk FreeRTOS task
 * 
 * Ini adalah pseudocode untuk menunjukkan cara menggunakan module ini
 * dalam aplikasi nyata dengan FreeRTOS.
 */

/* Global instances */
static System_Context_t g_system_ctx;
static PID_Controller_t g_pid_ctrl;
static EMA_Filter_t g_sensor_filter;

/**
 * @brief Main PID Control Task (FreeRTOS)
 */
void Task_PID_Control(void *pvParameters)
{
    ctrl_float_t setpoint = 100.0F;
    ctrl_float_t raw_sensor;
    ctrl_float_t filtered_sensor;
    ctrl_float_t pwm_output;
    bool start_cmd = false;
    bool estop_cmd = false;
    
    /* Initialize semua subsystems */
    SYS_Init(&g_system_ctx);
    
    /* Konfigurasi PID gains */
    g_pid_ctrl.kp = 2.0F;
    g_pid_ctrl.ki = 0.5F;
    g_pid_ctrl.kd = 1.0F;
    g_pid_ctrl.integral_limit = 50.0F;
    g_pid_ctrl.output_max = 100.0F;
    g_pid_ctrl.output_min = 0.0F;
    PID_Init(&g_pid_ctrl);
    
    /* Konfigurasi EMA filter (α = 0.3) */
    EMA_Init(&g_sensor_filter, 0.3F);
    
    /* Main control loop */
    for (;;) {
        /* Read digital inputs */
        start_cmd = read_start_button();  /* Implement sendiri */
        estop_cmd = read_estop_button();  /* Implement sendiri */
        
        /* Update FSM */
        g_system_ctx.current_state = SYS_Update(&g_system_ctx,
                                                 start_cmd,
                                                 estop_cmd);
        
        /* State-dependent behavior */
        switch (g_system_ctx.current_state) {
            case CTRL_STATE_INIT:
                /* Wait initialization complete */
                pwm_output = 0.0F;
                break;
                
            case CTRL_STATE_IDLE:
                /* System ready, wait for run command */
                PID_Reset(&g_pid_ctrl);
                pwm_output = 0.0F;
                break;
                
            case CTRL_STATE_RUNNING:
                /* Normal PID operation */
                raw_sensor = read_adc_channel(ADC_CHANNEL_0);
                filtered_sensor = EMA_Update(&g_sensor_filter, raw_sensor);
                pwm_output = PID_Update(&g_pid_ctrl, setpoint, filtered_sensor);
                break;
                
            case CTRL_STATE_ESTOP:
                /* Safe state: output = 0 */
                pwm_output = 0.0F;
                PID_Reset(&g_pid_ctrl);
                break;
                
            default:
                pwm_output = 0.0F;
                break;
        }
        
        /* Apply PWM output ke actuator */
        set_pwm_duty(pwm_output);  /* Implement sendiri */
        
        /* Delay untuk sampling period (contoh: 10ms) */
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

#endif  /* End conditional compilation */

/* ========================================================================= */
/* END OF FILE                                                               */
/* ========================================================================= */
