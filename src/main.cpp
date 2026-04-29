#include <Arduino.h>

#define LED_PIN 2
#define PWM_PIN 4
#define PULSE_PIN 5
#define FAST_PIN 18

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(PULSE_PIN, OUTPUT);
  pinMode(FAST_PIN, OUTPUT);
  
  // Setup PWM manual (50% duty cycle, 1kHz)
  ledcSetup(0, 1000, 8); // channel 0, 1kHz, 8-bit resolution
  ledcAttachPin(PWM_PIN, 0);
}

void loop() {
  static unsigned long lastTime = 0;
  static bool toggle = false;
  unsigned long now = millis();
  
  // UART: Kirim data
  Serial.print("T=");
  Serial.println(now);
  
  // LED blink (1 Hz)
  if (now - lastTime >= 500) {
    toggle = !toggle;
    digitalWrite(LED_PIN, toggle);
    lastTime = now;
    
    // Pulse singkat
    digitalWrite(PULSE_PIN, HIGH);
    delayMicroseconds(100);
    digitalWrite(PULSE_PIN, LOW);
  }
  
  // PWM otomatis (via ledc)
  ledcWrite(0, 128); // 50% duty cycle
  
  // Fast toggle (10kHz)
  digitalWrite(FAST_PIN, HIGH);
  delayMicroseconds(50);
  digitalWrite(FAST_PIN, LOW);
  delayMicroseconds(50);
  
  delay(100);
}