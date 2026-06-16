"""
Logic Analyzer Auto-Report Generator
Standardized workflow: CSV → Analysis → Multi-format Report
Compatible with PulseView/FX2LP exports
"""

import argparse
import logging
import sys
import os
import json
from datetime import datetime
from typing import Dict, List, Optional, Tuple

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np

# ─── CONFIGURATION CONSTANTS ─────────────────────────────────────────────
DEFAULT_SAMPLE_RATE_MHZ = 8
FREQ_THRESHOLD_PWM = 100.0      # Hz
FREQ_THRESHOLD_SLOW = 10.0      # Hz
UART_EDGE_THRESHOLD = 100       # Minimum edges to consider UART
JITTER_THRESHOLD_RATIO = 0.1    # std/mean ratio for UART detection
LOG_FORMAT = "%(asctime)s | %(levelname)-8s | %(message)s"

# ─── LOGGING SETUP ──────────────────────────────────────────────────────
logging.basicConfig(level=logging.INFO, format=LOG_FORMAT, handlers=[
    logging.StreamHandler(sys.stdout),
    logging.FileHandler("la_analysis.log", mode="a", encoding="utf-8")
])
logger = logging.getLogger(__name__)


class LogicAnalyzerArchiver:
    """Auto-generates analysis reports from Logic Analyzer CSV exports."""
    
    def __init__(self, csv_file: str, sample_rate_mhz: float = DEFAULT_SAMPLE_RATE_MHZ):
        self.csv_file = csv_file
        self.sample_rate_mhz = sample_rate_mhz
        self.df: pd.DataFrame = pd.DataFrame()
        self.results: Dict[str, dict] = {}
        
        # Auto-archive folder with timestamp
        self.archive_folder = f"Archive_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        os.makedirs(self.archive_folder, exist_ok=True)
        logger.info(f"Archive directory created: {self.archive_folder}/")

    def _load_and_normalize_csv(self) -> None:
        """Load CSV and normalize PulseView duplicate column names."""
        try:
            self.df = pd.read_csv(self.csv_file)
            cols = self.df.columns.tolist()
            if len(cols) > 1:
                self.df.columns = ['nanoseconds'] + [f'logic.{i}' for i in range(len(cols)-1)]
            self.df['Time_s'] = self.df['nanoseconds'] / 1e9
            logger.info(f"Loaded {len(self.df)} samples from {self.csv_file}")
        except Exception as e:
            logger.error(f"Failed to load CSV: {e}")
            sys.exit(1)

    def analyze_channel(self, channel_idx: int, channel_name: Optional[str] = None) -> Optional[dict]:
        """Analyze single channel: frequency, duty cycle, period, signal type."""
        if channel_name is None:
            channel_name = f'logic.{channel_idx}'
            
        if channel_name not in self.df.columns:
            return None
            
        signal = self.df[channel_name].values
        rising_edges = np.diff(signal) == 1
        edge_indices = np.where(rising_edges)[0]
        
        if len(edge_indices) < 2:
            return None
            
        periods = np.diff(self.df['Time_s'].values[edge_indices])
        mean_period = np.mean(periods)
        
        # ✅ FIX: Division by zero protection
        if mean_period <= 0:
            return None
            
        freq_avg = 1.0 / mean_period
        freq_std = np.std(1.0 / periods) if len(periods) > 1 else 0.0
        duty_cycle = float(np.mean(signal)) * 100.0
        
        # Heuristic signal classification
        if freq_avg > FREQ_THRESHOLD_PWM:
            signal_type = "PWM/Clock"
        elif freq_avg < FREQ_THRESHOLD_SLOW:
            signal_type = "Slow Pulse"
        else:
            signal_type = "Digital Signal"
            
        if len(edge_indices) > UART_EDGE_THRESHOLD and freq_avg > 0 and (freq_std / freq_avg) > JITTER_THRESHOLD_RATIO:
            signal_type = "UART/Data"
            
        result = {
            'channel': channel_name,
            'frequency_avg_hz': round(freq_avg, 2),
            'frequency_std_hz': round(freq_std, 2),
            'duty_cycle_percent': round(duty_cycle, 2),
            'total_edges': int(len(edge_indices)),
            'period_avg_us': round(np.mean(periods) * 1e6, 3),
            'period_std_us': round(np.std(periods) * 1e6, 3) if len(periods) > 1 else 0.0,
            'signal_type': signal_type,
            'high_count': int(np.sum(signal)),
            'low_count': int(len(signal) - np.sum(signal)),
            'jitter_ratio': round(freq_std / freq_avg, 4) if freq_avg > 0 else 0.0
        }
        self.results[channel_name] = result
        return result

    def plot_all_channels(self, channels_to_plot: Optional[List[str]] = None) -> None:
        """Generate professional waveform plot for all active channels."""
        if channels_to_plot is None:
            channels_to_plot = [f'logic.{i}' for i in range(8)]
            
        valid_channels = [ch for ch in channels_to_plot if ch in self.df.columns]
        if not valid_channels:
            logger.warning("No valid channels found for plotting.")
            return
            
        n = len(valid_channels)
        fig, axes = plt.subplots(n, 1, figsize=(12, 2.2*n), sharex=True, dpi=150)
        if n == 1:
            axes = [axes]
            
        time_ms = self.df['Time_s'].values * 1000
        
        for idx, ch in enumerate(valid_channels):
            ax = axes[idx]
            signal = self.df[ch].values
            
            # Step plot with edge markers
            ax.step(time_ms, signal, where='post', linewidth=0.8, color='#1f77b4')
            ax.plot(time_ms[1:][np.diff(signal) != 0], signal[1:][np.diff(signal) != 0], 
                    'r.', markersize=4, label='Edges')
            
            ax.set_ylabel(f'{ch}\nLogic Level', fontsize=9, rotation=0, labelpad=25, ha='right')
            ax.set_ylim(-0.2, 1.2)
            ax.set_yticks([0, 1])
            ax.grid(True, which='both', alpha=0.3, linestyle='--')
            ax.minorticks_on()
            
            if ch in self.results:
                r = self.results[ch]
                info = f"{r['signal_type']} | {r['frequency_avg_hz']:.1f} Hz | {r['duty_cycle_percent']:.1f}% DC"
                ax.set_title(info, fontsize=9, loc='right', fontweight='medium')
                
        axes[-1].set_xlabel('Time (ms)')
        plt.suptitle(f'Logic Analyzer Capture - {os.path.basename(self.csv_file)}', 
                     fontsize=12, fontweight='bold', y=0.98)
        plt.tight_layout()
        
        plot_path = os.path.join(self.archive_folder, 'all_channels_waveform.png')
        plt.savefig(plot_path, bbox_inches='tight')
        logger.info(f"Waveform plot saved: {plot_path}")
        plt.close()

    def generate_report(self) -> None:
        """Generate text-based analysis report."""
        report_path = os.path.join(self.archive_folder, 'analysis_report.txt')
        duration_ms = (self.df['Time_s'].max() - self.df['Time_s'].min()) * 1000
        
        with open(report_path, 'w', encoding='utf-8') as f:
            f.write("="*70 + "\n")
            f.write("LOGIC ANALYZER ANALYSIS REPORT\n")
            f.write("="*70 + "\n")
            f.write(f"Generated        : {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
            f.write(f"Source File      : {self.csv_file}\n")
            f.write(f"Total Samples    : {len(self.df)}\n")
            f.write(f"Duration         : {duration_ms:.3f} ms\n")
            f.write(f"Sample Rate      : {self.sample_rate_mhz} MHz\n")
            f.write("="*70 + "\n\n")
            
            f.write("CHANNEL ANALYSIS SUMMARY\n")
            f.write("-"*70 + "\n")
            
            for ch_name, res in self.results.items():
                f.write(f"\n{ch_name.upper()}\n")
                f.write(f"  Signal Type      : {res['signal_type']}\n")
                f.write(f"  Frequency        : {res['frequency_avg_hz']:.2f} Hz ± {res['frequency_std_hz']:.2f} Hz\n")
                f.write(f"  Duty Cycle       : {res['duty_cycle_percent']:.2f}%\n")
                f.write(f"  Period (avg)     : {res['period_avg_us']:.3f} µs ± {res['period_std_us']:.3f} µs\n")
                f.write(f"  Jitter Ratio     : {res['jitter_ratio']:.4f} {'(Stable)' if res['jitter_ratio'] < 0.05 else '(High Jitter)'}\n")
                f.write(f"  Total Edges      : {res['total_edges']}\n")
                f.write(f"  HIGH / LOW count : {res['high_count']} / {res['low_count']} samples\n")
                
            f.write("\n" + "="*70 + "\nEND OF REPORT\n" + "="*70 + "\n")
        logger.info(f"Report saved: {report_path}")

    def save_summary_csv(self) -> None:
        if not self.results: return
        summary_path = os.path.join(self.archive_folder, 'summary.csv')
        pd.DataFrame(self.results).T.to_csv(summary_path)
        logger.info(f"Summary CSV saved: {summary_path}")

    def save_metadata_json(self) -> None:
        metadata = {
            'timestamp': datetime.now().isoformat(),
            'source_file': self.csv_file,
            'total_samples': len(self.df),
            'duration_ms': (self.df['Time_s'].max() - self.df['Time_s'].min()) * 1000,
            'sample_rate_mhz': self.sample_rate_mhz,
            'channels_analyzed': list(self.results.keys()),
            'results': self.results
        }
        json_path = os.path.join(self.archive_folder, 'metadata.json')
        with open(json_path, 'w') as f:
            json.dump(metadata, f, indent=2)
        logger.info(f"Metadata JSON saved: {json_path}")

    def run_full_analysis(self, channels: Optional[List[str]] = None) -> None:
        if channels is None:
            channels = [f'logic.{i}' for i in range(8)]
            
        self._load_and_normalize_csv()
        logger.info("Analyzing channels...")
        for ch in channels:
            idx = int(ch.split('.')[1]) if '.' in ch else 0
            res = self.analyze_channel(idx, ch)
            if res:
                logger.info(f"  ✓ {ch}: {res['frequency_avg_hz']:.1f} Hz, {res['duty_cycle_percent']:.1f}% DC")
            else:
                logger.info(f"  ⚪ {ch}: (idle or no transitions)")
                
        logger.info("Generating plots and reports...")
        self.plot_all_channels(channels)
        self.generate_report()
        self.save_summary_csv()
        self.save_metadata_json()
        
        logger.info(f"✅ Archive complete! All files saved in: {self.archive_folder}/")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Logic Analyzer Auto-Report Generator")
    parser.add_argument("csv_file", nargs="?", 
                        default=os.path.join("software", "examples", "sample_capture.csv"),
                        help="Path to exported PulseView CSV")
    parser.add_argument("--sample-rate", type=float, default=DEFAULT_SAMPLE_RATE_MHZ,
                        help="Logic Analyzer sample rate in MHz (default: 8)")
    args = parser.parse_args()
    
    if not os.path.exists(args.csv_file):
        logger.error(f"File not found: {args.csv_file}")
        sys.exit(1)
        
    logger.info(f"Processing: {args.csv_file}")
    analyzer = LogicAnalyzerArchiver(args.csv_file, sample_rate_mhz=args.sample_rate)
    analyzer.run_full_analysis()