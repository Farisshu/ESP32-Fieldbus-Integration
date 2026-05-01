"""
Logic Analyzer Professional Report Generator
All-in-One Auto-Detection: UART, SPI, I2C, MCP2515, Modbus RTU

Workflow: Coding → Build → Test (LA + Serial) → Documentation
Standardized for R&D, Internship (HORIBA), and Industrial Automation

Author: M. Faris A. G.
Version: 1.0.1
License: MIT
"""

import argparse
import logging
import sys
import os
import json
import re
from datetime import datetime
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple, Any
from pathlib import Path

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# ─── CONFIGURATION & CONSTANTS ─────────────────────────────────────────────
VERSION = "1.0.1"
DEFAULT_SAMPLE_RATE_MHZ = 8.0

# Protocol thresholds
UART_BAUD_RATES = [9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600]
SPI_MAX_FREQ_MHZ = 10.0
I2C_STANDARD_FREQ_KHZ = 100
I2C_FAST_FREQ_KHZ = 400

# MCP2515 Register Map
MCP2515_REGISTERS = {
    0x0F: 'CANCTRL', 0x0E: 'CANSTAT',
    0x2A: 'CNF1', 0x29: 'CNF2', 0x28: 'CNF3',
    0x30: 'TXB0CTRL', 0x32: 'TXB0SIDH', 0x33: 'TXB0SIDL', 0x35: 'TXB0DLC',
    0x36: 'TXB0D0', 0x37: 'TXB0D1', 0x38: 'TXB0D2', 0x39: 'TXB0D3',
    0x3A: 'TXB0D4', 0x3B: 'TXB0D5', 0x3C: 'TXB0D6', 0x3D: 'TXB0D7',
    0x60: 'RXB0CTRL', 0x61: 'RXB0SIDH', 0x62: 'RXB0SIDL', 0x66: 'RXB0DLC',
    0x67: 'RXB0D0', 0x68: 'RXB0D1', 0x69: 'RXB0D2', 0x6A: 'RXB0D3',
    0x6B: 'RXB0D4', 0x6C: 'RXB0D5', 0x6D: 'RXB0D6', 0x6E: 'RXB0D7',
}

# Logging setup (Console only initially, File added later in Archive)
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(levelname)-8s | %(message)s",
    handlers=[logging.StreamHandler(sys.stdout)]
)
logger = logging.getLogger(__name__)


# ─── DATA CLASSES ──────────────────────────────────────────────────────
@dataclass
class TestConfig:
    operator: str = "Embedded Engineer"
    dut_name: str = "ESP32 Development Board"
    test_purpose: str = "Protocol Analysis & Timing Verification"
    sample_rate_mhz: float = DEFAULT_SAMPLE_RATE_MHZ
    voltage_threshold_v: float = 1.65
    trigger_channel: str = "auto"
    trigger_edge: str = "falling"


@dataclass
class ProtocolResult:
    protocol_type: str
    channel: str
    status: str = "ACTIVE"
    metrics: Dict = field(default_factory=dict)
    decoded_data: List = field(default_factory=list)
    errors: List = field(default_factory=list)
    validation: List = field(default_factory=list)


# ─── CSV LOADER (Auto-Detect Format) ───────────────────────────────────
class CaptureLoader:
    """Auto-detects and normalizes Logic Analyzer CSV exports."""
    
    @staticmethod
    def load(filepath: str) -> Tuple[pd.DataFrame, float]:
        """Load CSV and auto-detect format (PulseView/Saleae/Generic)."""
        path = Path(filepath)
        if not path.exists():
            raise FileNotFoundError(f"Capture file not found: {filepath}")
        
        # Read first few lines to detect format
        with open(path, 'r', encoding='utf-8', errors='ignore') as f:
            header = f.readline().strip()
        
        # Detect and parse
        if 'nanoseconds' in header.lower():
            logger.info("Detected PulseView format")
            return CaptureLoader._parse_pulseview(path)
        elif re.match(r'^time[,;]', header.lower(), re.I):
            logger.info("Detected Saleae/Generic format")
            return CaptureLoader._parse_generic(path, header)
        else:
            logger.warning("Unknown format, attempting generic parse")
            return CaptureLoader._parse_generic(path, header)

    @staticmethod
    def _parse_pulseview(path: Path) -> Tuple[pd.DataFrame, float]:
        df = pd.read_csv(path)
        cols = df.columns.tolist()
        time_ns = df[cols[0]]
        
        # Normalize column names
        data_cols = [f'ch{i}' for i in range(len(cols)-1)]
        df_norm = pd.DataFrame({'time_s': time_ns / 1e9})
        for i in range(len(data_cols)):
            df_norm[data_cols[i]] = df[cols[i+1]].astype(float)
        
        # Estimate sample rate
        dt = df_norm['time_s'].diff().dropna()
        sample_rate = 1.0 / dt.mean() if len(dt) > 0 else DEFAULT_SAMPLE_RATE_MHZ * 1e6
        
        return df_norm, sample_rate / 1e6  # Return in MHz

    @staticmethod
    def _parse_generic(path: Path, header: str) -> Tuple[pd.DataFrame, float]:
        df = pd.read_csv(path)
        df_norm = pd.DataFrame({'time_s': df.iloc[:, 0]})
        
        for i in range(1, df.shape[1]):
            df_norm[f'ch{i-1}'] = df.iloc[:, i].astype(float)
        
        dt = df_norm['time_s'].diff().dropna()
        sample_rate = 1.0 / dt.mean() if len(dt) > 0 else DEFAULT_SAMPLE_RATE_MHZ * 1e6
        
        return df_norm, sample_rate / 1e6


# ─── PROTOCOL AUTO-DETECTOR ────────────────────────────────────────────
class ProtocolDetector:
    """Auto-detects UART, SPI, I2C from waveform patterns."""
    
    @staticmethod
    def detect_all(df: pd.DataFrame, config: TestConfig) -> List[Dict]:
        """Detect all protocols present in the capture."""
        detected = []
        channels = [c for c in df.columns if c.startswith('ch')]
        
        # Detect UART (single channel with regular bit timing)
        for ch in channels:
            uart_info = ProtocolDetector._detect_uart(df, ch, config.sample_rate_mhz)
            if uart_info:
                detected.append({'type': 'UART', **uart_info})
        
        # Detect SPI (CS + CLK + MOSI + MISO pattern)
        if len(channels) >= 4:
            spi_info = ProtocolDetector._detect_spi(df, channels, config.sample_rate_mhz)
            if spi_info:
                detected.append({'type': 'SPI', **spi_info})
        
        # Detect I2C (SCL + SDA with open-drain pattern)
        if len(channels) >= 2:
            i2c_info = ProtocolDetector._detect_i2c(df, channels, config.sample_rate_mhz)
            if i2c_info:
                detected.append({'type': 'I2C', **i2c_info})
        
        return detected

    @staticmethod
    def _detect_uart(df: pd.DataFrame, channel: str, sample_rate_mhz: float) -> Optional[Dict]:
        """Detect UART by finding regular bit timing."""
        signal = df[channel].values
        edges = np.where(np.diff(signal) != 0)[0]
        
        if len(edges) < 10:  # Need enough edges
            return None
        
        # Calculate bit widths
        bit_widths = np.diff(edges) / (sample_rate_mhz * 1e6)  # in seconds
        
        # Find median bit width (should be ~1/baud)
        median_width = np.median(bit_widths)
        if median_width < 1e-6 or median_width > 1e-3:  # 1µs to 1ms
            return None
        
        # Estimate baud rate
        estimated_baud = int(1.0 / median_width)
        
        # Check if close to standard baud rates
        for std_baud in UART_BAUD_RATES:
            if abs(estimated_baud - std_baud) / std_baud < 0.05:  # 5% tolerance
                return {'channel': channel, 'baud_rate': std_baud, 'confidence': 'high'}
        
        return {'channel': channel, 'baud_rate': estimated_baud, 'confidence': 'low'}

    @staticmethod
    def _detect_spi(df: pd.DataFrame, channels: List[str], sample_rate_mhz: float) -> Optional[Dict]:
        """Detect SPI by finding clock + CS pattern."""
        # Look for channel with regular clock pattern (50% duty cycle approx)
        for ch in channels:
            signal = df[ch].values
            edges = np.where(np.diff(signal) != 0)[0]
            
            if len(edges) < 20:
                continue
            
            periods = np.diff(edges) / (sample_rate_mhz * 1e6)
            avg_period = np.mean(periods)
            freq = 1.0 / avg_period if avg_period > 0 else 0
            
            # Check for clock-like pattern (regular transitions)
            if 100e3 < freq < 20e6:  # 100kHz to 20MHz
                # Find CS channel (long low periods)
                cs_channel = ProtocolDetector._find_cs_channel(df, channels, ch)
                if cs_channel:
                    return {
                        'sck_channel': ch,
                        'cs_channel': cs_channel,
                        'frequency_hz': freq,
                        'channels': channels[:4]  # Assume first 4 are SPI
                    }
        
        return None

    @staticmethod
    def _find_cs_channel(df: pd.DataFrame, channels: List[str], sck_channel: str) -> Optional[str]:
        """Find Chip Select channel (active low, longer periods than clock)."""
        for ch in channels:
            if ch == sck_channel:
                continue
            
            signal = df[ch].values
            # CS should have longer low periods
            low_periods = np.where(signal == 0)[0]
            if len(low_periods) > 10:
                return ch
        
        return None

    @staticmethod
    def _detect_i2c(df: pd.DataFrame, channels: List[str], sample_rate_mhz: float) -> Optional[Dict]:
        """Detect I2C by finding SCL + SDA pattern with open-drain characteristics."""
        # Look for two channels with similar frequency but different patterns
        for i, ch1 in enumerate(channels[:2]):
            for ch2 in channels[i+1:3]:
                scl_sig = df[ch1].values
                sda_sig = df[ch2].values
                
                # Check for clock-like pattern on one channel
                scl_edges = np.where(np.diff(scl_sig) != 0)[0]
                if len(scl_edges) > 20:
                    periods = np.diff(scl_edges) / (sample_rate_mhz * 1e6)
                    freq = 1.0 / np.mean(periods) if len(periods) > 0 else 0
                    
                    if 50e3 < freq < 500e3:  # I2C range
                        return {
                            'scl_channel': ch1,
                            'sda_channel': ch2,
                            'frequency_hz': freq
                        }
        
        return None


# ─── PROTOCOL DECODERS ─────────────────────────────────────────────────
class UARTDecoder:
    """Decodes UART waveform into bytes."""
    
    def decode(self, df: pd.DataFrame, channel: str, baud_rate: int, sample_rate_mhz: float) -> ProtocolResult:
        """Decode UART signal into bytes."""
        signal = df[channel].values
        time = df['time_s'].values
        
        bit_time = 1.0 / baud_rate
        bytes_decoded = []
        errors = []
        
        # Find start bits (falling edges)
        edges = np.where(np.diff(signal) == -1)[0]
        
        for start_idx in edges:
            # Read 8 data bits + 1 stop bit
            byte_bits = []
            valid = True
            
            for bit_pos in range(10):  # 8 data + 1 parity (optional) + 1 stop
                sample_idx = int(start_idx + (bit_pos + 0.5) * bit_time * sample_rate_mhz * 1e6)
                if sample_idx >= len(signal):
                    valid = False
                    break
                byte_bits.append(int(signal[sample_idx]))
            
            if valid and len(byte_bits) >= 9:
                # Check stop bit
                if byte_bits[-1] != 1:
                    errors.append(f"Missing stop bit at sample {start_idx}")
                
                # Extract data byte (bits 1-8, LSB first)
                data_byte = sum(bit << (i-1) for i, bit in enumerate(byte_bits[1:9]))
                bytes_decoded.append(data_byte)
        
        return ProtocolResult(
            protocol_type='UART',
            channel=channel,
            metrics={'baud_rate': baud_rate, 'total_bytes': len(bytes_decoded)},
            decoded_data=bytes_decoded,
            errors=errors
        )


class SPIDecoder:
    """Decodes SPI transactions."""
    
    def decode(self, df: pd.DataFrame, sck_ch: str, mosi_ch: str, miso_ch: str, 
               cs_ch: str, sample_rate_mhz: float) -> ProtocolResult:
        """Decode SPI transactions."""
        sck = df[sck_ch].values
        mosi = df[mosi_ch].values
        miso = df[miso_ch].values
        cs = df[cs_ch].values
        
        transactions = []
        current_tx = {'mosi_bytes': [], 'miso_bytes': [], 'start_time': 0}
        
        # Find CS falling edges (transaction start)
        cs_falling = np.where(np.diff(cs) == -1)[0]
        cs_rising = np.where(np.diff(cs) == 1)[0]
        
        for start, end in zip(cs_falling, cs_rising[:len(cs_falling)]):
            # Extract bytes during CS low
            tx_sck = sck[start:end]
            tx_mosi = mosi[start:end]
            tx_miso = miso[start:end]
            
            # Find clock edges
            clk_edges = np.where(np.diff(tx_sck) != 0)[0]
            
            # Read bytes (8 bits per byte, MSB first)
            for i in range(0, len(clk_edges)-8, 8):
                if i+8 < len(clk_edges):
                    mosi_byte = 0
                    miso_byte = 0
                    
                    for bit in range(8):
                        idx = clk_edges[i+bit]
                        if idx < len(tx_mosi):
                            mosi_byte |= (int(tx_mosi[idx]) << (7-bit))
                        if idx < len(tx_miso):
                            miso_byte |= (int(tx_miso[idx]) << (7-bit))
                    
                    current_tx['mosi_bytes'].append(mosi_byte)
                    current_tx['miso_bytes'].append(miso_byte)
            
            if current_tx['mosi_bytes']:
                transactions.append(current_tx.copy())
                current_tx = {'mosi_bytes': [], 'miso_bytes': [], 'start_time': 0}
        
        return ProtocolResult(
            protocol_type='SPI',
            channel=f'{cs_ch}-{sck_ch}',
            metrics={'total_transactions': len(transactions)},
            decoded_data=transactions,
            errors=[]
        )


class MCP2515Parser:
    """Parses MCP2515 SPI transactions."""
    
    def parse(self, spi_result: ProtocolResult) -> ProtocolResult:
        """Interpret SPI data as MCP2515 commands."""
        transactions = spi_result.decoded_data
        parsed_commands = []
        register_writes = {}
        register_reads = {}
        
        for tx in transactions:
            mosi = tx['mosi_bytes']
            miso = tx['miso_bytes']
            
            if not mosi:
                continue
            
            cmd = mosi[0]
            
            # MCP2515 Commands
            if cmd == 0x02 and len(mosi) >= 3:  # WRITE
                addr = mosi[1]
                data = mosi[2]
                reg_name = MCP2515_REGISTERS.get(addr, f'0x{addr:02X}')
                parsed_commands.append({
                    'command': 'WRITE',
                    'register': reg_name,
                    'address': addr,
                    'value': data
                })
                register_writes[reg_name] = data
                
            elif cmd == 0x03 and len(miso) >= 2:  # READ
                addr = mosi[1] if len(mosi) > 1 else 0
                data = miso[1] if len(miso) > 1 else 0
                reg_name = MCP2515_REGISTERS.get(addr, f'0x{addr:02X}')
                parsed_commands.append({
                    'command': 'READ',
                    'register': reg_name,
                    'address': addr,
                    'value': data
                })
                register_reads[reg_name] = data
                
            elif cmd == 0xC0:  # RESET
                parsed_commands.append({'command': 'RESET'})
        
        # Determine mode
        canctrl = register_writes.get('CANCTRL', 0)
        mode = 'Normal' if canctrl == 0x00 else \
               'Configuration' if canctrl == 0x80 else \
               'Loopback' if canctrl == 0x40 else f'Unknown (0x{canctrl:02X})'
        
        return ProtocolResult(
            protocol_type='MCP2515',
            channel=spi_result.channel,
            status='ACTIVE',
            metrics={
                'total_commands': len(parsed_commands),
                'mode': mode,
                'registers_written': len(register_writes),
                'registers_read': len(register_reads)
            },
            decoded_data=parsed_commands,
            errors=[],
            validation=[f"Mode: {mode}"]
        )


class I2CDecoder:
    """Decodes I2C transactions."""
    
    def decode(self, df: pd.DataFrame, scl_ch: str, sda_ch: str, 
               sample_rate_mhz: float) -> ProtocolResult:
        """Decode I2C signal into frames."""
        scl = df[scl_ch].values
        sda = df[sda_ch].values
        
        frames = []
        current_frame = {'address': None, 'rw': None, 'data': [], 'ack': []}
        
        # Find START conditions (SDA falling while SCL high)
        for i in range(1, len(scl)-1):
            if scl[i] == 1 and sda[i-1] == 1 and sda[i] == 0:
                # START detected, read address
                if i+8 < len(scl):
                    addr = 0
                    for bit in range(7):
                        idx = i + 1 + bit
                        if idx < len(scl) and scl[idx] == 1:
                            addr |= (int(sda[idx]) << (6-bit))
                    
                    # R/W bit
                    rw_idx = i + 8
                    rw = int(sda[rw_idx]) if rw_idx < len(sda) else 0
                    
                    current_frame = {'address': addr, 'rw': 'R' if rw else 'W', 'data': []}
        
        return ProtocolResult(
            protocol_type='I2C',
            channel=f'{scl_ch}-{sda_ch}',
            metrics={'total_frames': len(frames)},
            decoded_data=frames,
            errors=[]
        )


# ─── MODBUS RTU PARSER ─────────────────────────────────────────────────
class ModbusParser:
    """Parses Modbus RTU frames from UART data."""
    
    @staticmethod
    def calculate_crc16(data: List[int]) -> int:
        """Calculate Modbus CRC-16."""
        crc = 0xFFFF
        for byte in data:
            crc ^= byte
            for _ in range(8):
                if crc & 0x0001:
                    crc = (crc >> 1) ^ 0xA001
                else:
                    crc >>= 1
        return crc
    
    def parse(self, uart_result: ProtocolResult) -> ProtocolResult:
        """Parse UART bytes as Modbus RTU frames."""
        bytes_data = uart_result.decoded_data
        frames = []
        
        i = 0
        while i < len(bytes_data) - 2:
            # Find frame start (slave address)
            addr = bytes_data[i]
            if 0 < addr <= 247:  # Valid Modbus address
                func_code = bytes_data[i+1] if i+1 < len(bytes_data) else 0
                
                # Determine frame length based on function code
                if func_code in [0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x0F, 0x10]:
                    frame_len = 8  # Standard request
                elif func_code in [0x81, 0x82, 0x83, 0x84]:  # Exception
                    frame_len = 5
                else:
                    frame_len = 4  # Minimum
                
                if i + frame_len <= len(bytes_data):
                    frame_bytes = bytes_data[i:i+frame_len]
                    
                    # Extract CRC
                    crc_received = (bytes_data[i+frame_len-1] << 8) | bytes_data[i+frame_len-2]
                    crc_calculated = self.calculate_crc16(frame_bytes[:-2])
                    
                    crc_valid = (crc_received == crc_calculated)
                    
                    frames.append({
                        'address': addr,
                        'function_code': func_code,
                        'data': frame_bytes[2:-2].hex() if len(frame_bytes) > 4 else '',
                        'crc_received': f'0x{crc_received:04X}',
                        'crc_calculated': f'0x{crc_calculated:04X}',
                        'crc_valid': crc_valid
                    })
                    
                    i += frame_len
                else:
                    i += 1
            else:
                i += 1
        
        return ProtocolResult(
            protocol_type='Modbus RTU',
            channel=uart_result.channel,
            metrics={'total_frames': len(frames)},
            decoded_data=frames,
            errors=[f"CRC error in frame {i}" for i, f in enumerate(frames) if not f['crc_valid']]
        )


# ─── PROFESSIONAL REPORT GENERATOR ─────────────────────────────────────
class ReportGenerator:
    """Generates comprehensive professional reports."""
    
    def __init__(self, config: TestConfig, df: pd.DataFrame, 
                 detected_protocols: List[Dict], results: Dict[str, ProtocolResult]):
        self.config = config
        self.df = df
        self.detected = detected_protocols
        self.results = results
        self.archive_dir = f"Archive_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        os.makedirs(self.archive_dir, exist_ok=True)
        
        # 📝 FIX: Pindahkan log ke dalam folder archive
        log_path = os.path.join(self.archive_dir, 'la_analysis.log')
        file_handler = logging.FileHandler(log_path, mode='a', encoding='utf-8')
        file_handler.setFormatter(logging.Formatter("%(asctime)s | %(levelname)-8s | %(message)s"))
        logger.addHandler(file_handler)
        logger.info(f"Log initialized inside archive: {log_path}")
        
    def generate_all(self) -> None:
        """Generate all report components."""
        self._generate_plot()
        self._generate_text_report()
        self._save_metadata()
        logger.info(f"📁 Report saved to: {self.archive_dir}/")
        
    def _generate_plot(self) -> None:
        """Generate annotated waveform plot."""
        channels = [c for c in self.df.columns if c.startswith('ch')]
        active_channels = []
        
        # Only plot channels with activity
        for ch in channels:
            signal = self.df[ch].values
            if np.any(np.diff(signal) != 0):  # Has transitions
                active_channels.append(ch)
        
        if not active_channels:
            logger.warning("No active channels to plot.")
            return
        
        fig, axes = plt.subplots(len(active_channels), 1, figsize=(14, 2.5*len(active_channels)), 
                                 sharex=True, dpi=150)
        if len(active_channels) == 1:
            axes = [axes]
        
        time_ms = self.df['time_s'].values * 1000
        
        for idx, ch in enumerate(active_channels):
            ax = axes[idx]
            signal = self.df[ch].values
            
            # Step plot
            ax.step(time_ms, signal, where='post', linewidth=0.8, color='#1f77b4')
            
            # Edge markers
            edges = np.where(np.diff(signal) != 0)[0]
            ax.plot(time_ms[edges], signal[edges], 'ro', markersize=4, alpha=0.7, label='Edges')
            
            # Add protocol annotations if available
            for proto_name, proto_result in self.results.items():
                if proto_result.channel == ch or ch in str(proto_result.channel):
                    ax.text(0.02, 0.95, f"{proto_name}", transform=ax.transAxes, 
                           fontsize=9, fontweight='bold', color='darkgreen',
                           bbox=dict(facecolor='white', alpha=0.8))
            
            ax.set_ylabel(f'{ch}\nLogic', fontsize=9, rotation=0, labelpad=20, ha='right')
            ax.set_ylim(-0.2, 1.2)
            ax.set_yticks([0, 1])
            ax.grid(True, alpha=0.3, linestyle='--')
            ax.legend(loc='upper right', fontsize=8)
        
        axes[-1].set_xlabel('Time (ms)')
        plt.suptitle(f'LA Capture: {self.config.dut_name} | {self.config.test_purpose}', 
                    fontsize=12, fontweight='bold', y=0.98)
        plt.tight_layout()
        
        plot_path = os.path.join(self.archive_dir, 'waveform_annotated.png')
        plt.savefig(plot_path, bbox_inches='tight')
        plt.close()
        logger.info(f"📊 Waveform saved: {plot_path}")
    
    def _generate_text_report(self) -> None:
        """Generate comprehensive text report."""
        duration = (self.df['time_s'].max() - self.df['time_s'].min()) * 1000
        
        report_path = os.path.join(self.archive_dir, 'professional_report.txt')
        with open(report_path, 'w', encoding='utf-8') as f:
            # Header
            f.write("═"*70 + "\n")
            f.write("LOGIC ANALYZER PROFESSIONAL TEST REPORT\n")
            f.write(f"Generated by: analyze_la_pro.py v{VERSION}\n")
            f.write("═"*70 + "\n\n")
            
            # 1. Metadata
            f.write("📋 METADATA\n")
            f.write(f"  Date/Time        : {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
            f.write(f"  Operator         : {self.config.operator}\n")
            f.write(f"  Device Under Test: {self.config.dut_name}\n")
            f.write(f"  Test Purpose     : {self.config.test_purpose}\n")
            f.write(f"  Report Version   : {VERSION}\n\n")
            
            # 2. Measurement Setup
            f.write("⚙️  MEASUREMENT SETUP\n")
            f.write(f"  Sample Rate      : {self.config.sample_rate_mhz} MHz\n")
            f.write(f"  Capture Duration : {duration:.3f} ms\n")
            f.write(f"  Voltage Threshold: {self.config.voltage_threshold_v} V (TTL)\n")
            f.write(f"  Channels Active  : {len([c for c in self.df.columns if c.startswith('ch')])}\n")
            f.write(f"  Protocols Detected: {', '.join(p['type'] for p in self.detected)}\n\n")
            
            # 3. Protocol Analysis Results
            f.write("🔍 PROTOCOL ANALYSIS RESULTS\n")
            f.write("-"*70 + "\n")
            
            for proto_name, result in self.results.items():
                f.write(f"\n  {proto_name.upper()}\n")
                f.write(f"  Channel(s)       : {result.channel}\n")
                f.write(f"  Status           : {result.status}\n")
                
                # Metrics
                for key, value in result.metrics.items():
                    f.write(f"  {key.replace('_', ' ').title():18}: {value}\n")
                
                # Validation
                if result.validation:
                    f.write("  Validation:\n")
                    for v in result.validation:
                        f.write(f"    ✓ {v}\n")
                
                # Errors
                if result.errors:
                    f.write("  Errors:\n")
                    for e in result.errors[:5]:  # Show first 5 errors
                        f.write(f"    ✗ {e}\n")
                    if len(result.errors) > 5:
                        f.write(f"    ... and {len(result.errors)-5} more\n")
            
            f.write("\n")
            
            # 4. Decoded Data Summary
            f.write("📊 DECODED DATA SUMMARY\n")
            f.write("-"*70 + "\n")
            
            for proto_name, result in self.results.items():
                if result.decoded_data:
                    f.write(f"\n  {proto_name} Data:\n")
                    if proto_name == 'UART':
                        # Show as hex + ASCII
                        bytes_data = result.decoded_data[:32]  # First 32 bytes
                        hex_str = ' '.join(f'{b:02X}' for b in bytes_data)
                        ascii_str = ''.join(chr(b) if 32 <= b < 127 else '.' for b in bytes_data)
                        f.write(f"    Hex  : {hex_str}\n")
                        f.write(f"    ASCII: {ascii_str}\n")
                    
                    elif proto_name == 'MCP2515':
                        # Show register operations
                        for cmd in result.decoded_data[:10]:  # First 10 commands
                            if cmd['command'] == 'WRITE':
                                f.write(f"    WRITE {cmd['register']:12} = 0x{cmd['value']:02X}\n")
                            elif cmd['command'] == 'READ':
                                f.write(f"    READ  {cmd['register']:12} ← 0x{cmd['value']:02X}\n")
                            elif cmd['command'] == 'RESET':
                                f.write(f"    RESET\n")
                    
                    elif proto_name == 'Modbus RTU':
                        # Show Modbus frames
                        for frame in result.decoded_data[:10]:
                            crc_status = "✓" if frame['crc_valid'] else "✗"
                            f.write(f"    Addr={frame['address']:02X} Func=0x{frame['function_code']:02X} "
                                   f"Data={frame['data']} CRC={frame['crc_received']} {crc_status}\n")
            
            f.write("\n")
            
            # 5. Visual Evidence
            f.write("🖼️  VISUAL EVIDENCE\n")
            f.write("  • Annotated waveform: waveform_annotated.png\n")
            f.write(f"  • Timing resolution  : ~{1000/self.config.sample_rate_mhz:.1f} ns @ {self.config.sample_rate_mhz} MHz\n\n")
            
            # 6. Conclusions & Recommendations
            f.write("✅ CONCLUSIONS & RECOMMENDATIONS\n")
            f.write("-"*70 + "\n")
            
            # Auto-generate conclusions
            has_errors = any(len(r.errors) > 0 for r in self.results.values())
            
            if not has_errors:
                f.write("  ✅ PASS: All protocols decoded successfully without errors.\n")
                f.write("  ✅ Signal integrity appears stable.\n")
            else:
                f.write("  ⚠️  WARNING: Some errors detected during decoding.\n")
                f.write("  🔧 Recommendations:\n")
                f.write("    • Check wiring and connections\n")
                f.write("    • Verify baud rate / clock frequency settings\n")
                f.write("    • Review termination resistors for signal integrity\n")
            
            f.write("\n  Next Steps:\n")
            f.write("    • Cross-verify with oscilloscope for analog characteristics\n")
            f.write("    • Capture longer duration for statistical analysis\n")
            f.write("    • Export raw CSV + this report to version control\n")
            
            f.write("\n" + "═"*70 + "\n")
            f.write("END OF REPORT\n")
            f.write("═"*70 + "\n")
        
        logger.info(f"📄 Text report saved: {report_path}")
    
    def _save_metadata(self) -> None:
        """Save metadata as JSON."""
        metadata = {
            'timestamp': datetime.now().isoformat(),
            'version': VERSION,
            'config': {
                'operator': self.config.operator,
                'dut_name': self.config.dut_name,
                'test_purpose': self.config.test_purpose,
                'sample_rate_mhz': self.config.sample_rate_mhz,
                'voltage_threshold_v': self.config.voltage_threshold_v
            },
            'detected_protocols': self.detected,
            'results': {
                name: {
                    'protocol_type': r.protocol_type,
                    'channel': r.channel,
                    'status': r.status,
                    'metrics': r.metrics,
                    'error_count': len(r.errors)
                }
                for name, r in self.results.items()
            }
        }
        
        with open(os.path.join(self.archive_dir, 'metadata.json'), 'w') as f:
            json.dump(metadata, f, indent=2)


# ─── MAIN ANALYZER ─────────────────────────────────────────────────────
class LogicAnalyzerPro:
    """Main all-in-one analyzer."""
    
    def __init__(self, csv_file: str, config: TestConfig):
        self.csv_file = csv_file
        self.config = config
        self.df = pd.DataFrame()
        self.detected_protocols: List[Dict] = []
        self.results: Dict[str, ProtocolResult] = {}
        
    def load_capture(self) -> None:
        """Load and normalize CSV capture."""
        logger.info(f"📂 Loading capture: {self.csv_file}")
        self.df, detected_rate = CaptureLoader.load(self.csv_file)
        
        if abs(detected_rate - self.config.sample_rate_mhz) > 1.0:
            logger.warning(f"⚠️  Detected sample rate ({detected_rate:.1f} MHz) differs from config. Using detected.")
            self.config.sample_rate_mhz = round(detected_rate, 1)
        
        logger.info(f"  Loaded {len(self.df)} samples @ {self.config.sample_rate_mhz} MHz")
    
    def detect_protocols(self) -> None:
        """Auto-detect protocols in capture."""
        logger.info("🔍 Auto-detecting protocols...")
        self.detected_protocols = ProtocolDetector.detect_all(self.df, self.config)
        
        if self.detected_protocols:
            for proto in self.detected_protocols:
                logger.info(f"  ✓ Detected: {proto['type']} on {proto.get('channel', 'multiple channels')}")
        else:
            logger.warning("  ⚠️  No protocols auto-detected. Will analyze all channels.")
    
    def decode_all(self) -> None:
        """Decode all detected protocols."""
        logger.info(" Decoding protocols...")
        
        for proto in self.detected_protocols:
            proto_type = proto['type']
            
            try:
                if proto_type == 'UART':
                    decoder = UARTDecoder()
                    result = decoder.decode(self.df, proto['channel'], 
                                          proto.get('baud_rate', 9600), 
                                          self.config.sample_rate_mhz)
                    self.results['UART'] = result
                    
                    # Also try Modbus parsing
                    modbus_parser = ModbusParser()
                    modbus_result = modbus_parser.parse(result)
                    if modbus_result.decoded_data:
                        self.results['Modbus RTU'] = modbus_result
                        logger.info(f"  ✓ Decoded Modbus RTU: {len(modbus_result.decoded_data)} frames")
                    
                    logger.info(f"  ✓ Decoded UART: {len(result.decoded_data)} bytes @ {proto.get('baud_rate', 9600)} baud")
                
                elif proto_type == 'SPI':
                    decoder = SPIDecoder()
                    result = decoder.decode(
                        self.df, 
                        proto.get('sck_channel', 'ch1'),
                        proto.get('mosi_channel', 'ch2'),
                        proto.get('miso_channel', 'ch3'),
                        proto.get('cs_channel', 'ch0'),
                        self.config.sample_rate_mhz
                    )
                    self.results['SPI'] = result
                    
                    # Try MCP2515 parsing
                    mcp_parser = MCP2515Parser()
                    mcp_result = mcp_parser.parse(result)
                    if mcp_result.decoded_data:
                        self.results['MCP2515'] = mcp_result
                        logger.info(f"  ✓ Parsed MCP2515: {mcp_result.metrics.get('mode', 'unknown')} mode")
                    
                    logger.info(f"  ✓ Decoded SPI: {result.metrics['total_transactions']} transactions")
                
                elif proto_type == 'I2C':
                    decoder = I2CDecoder()
                    result = decoder.decode(
                        self.df,
                        proto.get('scl_channel', 'ch0'),
                        proto.get('sda_channel', 'ch1'),
                        self.config.sample_rate_mhz
                    )
                    self.results['I2C'] = result
                    logger.info(f"  ✓ Decoded I2C: {len(result.decoded_data)} frames")
            
            except Exception as e:
                logger.error(f"  ✗ Error decoding {proto_type}: {e}")
                import traceback
                traceback.print_exc()
    
    def generate_report(self) -> None:
        """Generate professional report."""
        logger.info("📝 Generating professional report...")
        generator = ReportGenerator(self.config, self.df, self.detected_protocols, self.results)
        generator.generate_all()
    
    def run_full_analysis(self) -> None:
        """Run complete analysis pipeline."""
        self.load_capture()
        self.detect_protocols()
        self.decode_all()
        self.generate_report()
        
        logger.info("✅ Analysis complete!")


# ─── CLI ENTRY POINT ───────────────────────────────────────────────────
def main():
    parser = argparse.ArgumentParser(
        description=f"Logic Analyzer Professional Report Generator v{VERSION}",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s capture.csv
  %(prog)s capture.csv --operator "John Doe" --dut "ESP32+MCP2515"
  %(prog)s capture.csv --purpose "CAN Bus Verification" --sample-rate 24
        """
    )
    
    parser.add_argument("csv_file", help="Path to Logic Analyzer CSV capture")
    parser.add_argument("--operator", default="Embedded Engineer", 
                       help="Operator name (default: Embedded Engineer)")
    parser.add_argument("--dut", default="ESP32 Development Board",
                       help="Device Under Test (default: ESP32 Development Board)")
    parser.add_argument("--purpose", default="Protocol Analysis & Timing Verification",
                       help="Test purpose (default: Protocol Analysis...)")
    parser.add_argument("--sample-rate", type=float, default=DEFAULT_SAMPLE_RATE_MHZ,
                       help=f"Sample rate in MHz (default: {DEFAULT_SAMPLE_RATE_MHZ})")
    parser.add_argument("--voltage", type=float, default=1.65,
                       help="Voltage threshold in V (default: 1.65)")
    parser.add_argument("--version", action='version', version=f'%(prog)s {VERSION}')
    
    args = parser.parse_args()
    
    if not os.path.exists(args.csv_file):
        logger.error(f"File not found: {args.csv_file}")
        sys.exit(1)
    
    config = TestConfig(
        operator=args.operator,
        dut_name=args.dut,
        test_purpose=args.purpose,
        sample_rate_mhz=args.sample_rate,
        voltage_threshold_v=args.voltage
    )
    
    try:
        analyzer = LogicAnalyzerPro(args.csv_file, config)
        analyzer.run_full_analysis()
    except Exception as e:
        logger.error(f"Analysis failed: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)


if __name__ == "__main__":
    main()