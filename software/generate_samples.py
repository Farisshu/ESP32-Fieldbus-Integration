"""
Script untuk generate Sample CSV (PulseView Format)
Untuk testing fitur Auto-Detect & Decoder
"""

import os
import csv

OUTPUT_DIR = "software/examples"
SAMPLE_RATE_MHZ = 8  # 1 sample = 125 ns
SAMPLE_PERIOD_NS = 125

def write_header(f):
    f.write("nanoseconds,logic,logic,logic,logic,logic,logic,logic,logic\n")

def write_row(f, time_ns, *channels):
    ch_str = ",".join(map(str, channels))
    f.write(f"{time_ns},{ch_str}\n")

def generate_uart_sample():
    """Generate UART 9600 Baud, 8N1 - Multiple bytes for detection"""
    print("Generating sample_uart_9600.csv...")
    filepath = os.path.join(OUTPUT_DIR, "sample_uart_9600.csv")
    
    with open(filepath, 'w', newline='') as f:
        write_header(f)
        
        # UART Config: 9600 baud = 1 bit every ~104.16 us
        # Dengan sample rate 8MHz (0.125us per sample)
        # 1 bit = ~833 samples.
        # Generate multiple bytes 'U' (0x55 = 01010101) untuk edge yang cukup
        
        current_ns = 0
        
        # Generate 10 bytes of 0x55 untuk testing (total ~80 edges)
        pattern = [1,0,1,0,1,0,1,0]  # 0x55 LSB first
        for byte_num in range(10):
            # Start Bit (0)
            for _ in range(833):
                write_row(f, current_ns, 0, 1, 1, 1, 1, 1, 1, 1)
                current_ns += SAMPLE_PERIOD_NS
                
            # Data Bits
            for bit in pattern:
                for _ in range(833):
                    write_row(f, current_ns, bit, 1, 1, 1, 1, 1, 1, 1)
                    current_ns += SAMPLE_PERIOD_NS
                    
            # Stop Bit (1)
            for _ in range(833):
                write_row(f, current_ns, 1, 1, 1, 1, 1, 1, 1, 1)
                current_ns += SAMPLE_PERIOD_NS
        
        # Idle time
        for _ in range(1000):
            write_row(f, current_ns, 1, 1, 1, 1, 1, 1, 1, 1)
            current_ns += SAMPLE_PERIOD_NS
            
    print(f"Saved to {filepath}")

def generate_spi_mcp2515_sample():
    """Generate SPI Transaction (Write CANCTRL)"""
    print("Generating sample_spi_mcp2515.csv...")
    filepath = os.path.join(OUTPUT_DIR, "sample_spi_mcp2515.csv")
    
    # ch0: CS, ch1: SCK, ch2: MOSI, ch3: MISO
    with open(filepath, 'w', newline='') as f:
        write_header(f)
        
        current_ns = 0
        idle_time = 100 # samples
        
        # --- TRANSACTION START ---
        # 1. CS Pull Down (Active Low)
        for _ in range(50): 
            write_row(f, current_ns, 1, 0, 1, 1, 1, 1, 1, 1) # CS=1, SCK=0
            current_ns += SAMPLE_PERIOD_NS
            
        for _ in range(50):
            write_row(f, current_ns, 0, 0, 1, 1, 1, 1, 1, 1) # CS=0
            current_ns += SAMPLE_PERIOD_NS
            
        # 2. Clocking Data (Write 0x02, Addr 0x0F, Data 0x80)
        # Command: 0000 0010
        # Address: 0000 1111
        # Data:    1000 0000
        
        # Helper untuk write 1 bit SPI
        def write_spi_bit(clk_val, mosi_val, miso_val):
            nonlocal current_ns
            # Clock Low
            write_row(f, current_ns, 0, 0, mosi_val, miso_val, 1, 1, 1, 1)
            current_ns += SAMPLE_PERIOD_NS * 10
            # Clock High (Data sampled here)
            write_row(f, current_ns, 0, 1, mosi_val, miso_val, 1, 1, 1, 1)
            current_ns += SAMPLE_PERIOD_NS * 10

        # Command Byte: 0x02 (Write)
        for bit in [0,0,0,0,0,0,1,0]:
            write_spi_bit(bit, bit, 0) # MOSI sends bit, MISO dummy 0
            
        # Address Byte: 0x0F (CANCTRL)
        for bit in [0,0,0,0,1,1,1,1]:
            write_spi_bit(bit, bit, 0)
            
        # Data Byte: 0x80 (Config Mode)
        for bit in [1,0,0,0,0,0,0,0]:
            write_spi_bit(bit, bit, 0)
            
        # --- TRANSACTION END ---
        # 3. CS Pull Up
        for _ in range(50):
            write_row(f, current_ns, 1, 0, 1, 1, 1, 1, 1, 1)
            current_ns += SAMPLE_PERIOD_NS
            
    print(f"Saved to {filepath}")

if __name__ == "__main__":
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    generate_uart_sample()
    generate_spi_mcp2515_sample()
    print("\n✅ Done! Sample files created in software/examples/")