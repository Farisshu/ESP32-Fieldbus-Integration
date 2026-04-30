import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from datetime import datetime
import os
import json

class LogicAnalyzerArchiver:
    def __init__(self, csv_file):
        self.csv_file = csv_file
        self.df = pd.read_csv(csv_file)
        
        # 🛠️ FIX: Normalisasi nama kolom PulseView (sering duplikat 'logic')
        cols = self.df.columns.tolist()
        if len(cols) > 1:
            self.df.columns = ['nanoseconds'] + [f'logic.{i}' for i in range(len(cols)-1)]
            
        self.df['Time_s'] = self.df['nanoseconds'] / 1e9
        self.results = {}
        
        # Buat folder arsip otomatis
        self.archive_folder = f"Archive_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        os.makedirs(self.archive_folder, exist_ok=True)
        print(f"📁 Archive folder created: {self.archive_folder}/")
    
    def analyze_channel(self, channel_idx, channel_name=None):
        if channel_name is None:
            channel_name = f'logic.{channel_idx}'
            
        if channel_name not in self.df.columns:
            return None
            
        signal = self.df[channel_name]
        rising_edges = np.diff(signal) == 1
        edge_indices = np.where(rising_edges)[0]
        
        if len(edge_indices) > 1:
            periods = np.diff(self.df['Time_s'].values[edge_indices])
            freq_avg = 1 / np.mean(periods)
            freq_std = np.std(1 / periods) if len(periods) > 1 else 0
            duty_cycle = signal.mean() * 100
            
            # Deteksi tipe sinyal
            if freq_avg > 100: signal_type = "PWM/Clock"
            elif freq_avg < 10: signal_type = "Slow Pulse"
            else: signal_type = "Digital Signal"
            
            if len(edge_indices) > 100 and freq_std > freq_avg * 0.1:
                signal_type = "UART/Data"
                
            result = {
                'channel': channel_name,
                'frequency_avg_hz': round(freq_avg, 2),
                'frequency_std_hz': round(freq_std, 2),
                'duty_cycle_percent': round(duty_cycle, 2),
                'total_edges': int(len(edge_indices)),
                'period_avg_us': round(np.mean(periods) * 1e6, 3),
                'period_std_us': round(np.std(periods) * 1e6, 3) if len(periods) > 1 else 0,
                'signal_type': signal_type,
                'high_count': int(signal.sum()),
                'low_count': int(len(signal) - signal.sum()),
            }
            self.results[channel_name] = result
            return result
        return None
    
    def plot_all_channels(self, channels_to_plot=None):
        if channels_to_plot is None:
            channels_to_plot = [f'logic.{i}' for i in range(8)]
            
        # 🛠️ FIX: Hanya plot channel yang benar-benar ada di CSV
        valid_channels = [ch for ch in channels_to_plot if ch in self.df.columns]
        n_channels = len(valid_channels)
        if n_channels == 0:
            print("⚠️ No valid channels found for plotting.")
            return
            
        fig, axes = plt.subplots(n_channels, 1, figsize=(12, 2*n_channels), sharex=True)
        if n_channels == 1:
            axes = [axes]
            
        # 🛠️ FIX: Iterasi aman sesuai jumlah axes
        for idx, channel in enumerate(valid_channels):
            ax = axes[idx]
            time_ms = self.df['Time_s'].values * 1000
            signal = self.df[channel].values
            
            ax.step(time_ms, signal, where='post', linewidth=0.8)
            ax.set_ylabel(f'{channel}\nLogic Level', fontsize=9)
            ax.grid(True, alpha=0.3)
            ax.set_ylim(-0.2, 1.2)
            ax.set_yticks([0, 1])
            
            if channel in self.results:
                r = self.results[channel]
                info = f"{r['signal_type']} | {r['frequency_avg_hz']:.1f} Hz | {r['duty_cycle_percent']:.1f}%"
                ax.set_title(info, fontsize=9, loc='right')
                
        axes[-1].set_xlabel('Time (ms)')
        plt.suptitle(f'Logic Analyzer Capture - {os.path.basename(self.csv_file)}', fontsize=12, fontweight='bold')
        plt.tight_layout()
        
        plot_file = os.path.join(self.archive_folder, 'all_channels_waveform.png')
        plt.savefig(plot_file, dpi=150, bbox_inches='tight')
        print(f" Waveform plot saved: {plot_file}")
        plt.close()
    
    def generate_report(self):
        report_file = os.path.join(self.archive_folder, 'analysis_report.txt')
        with open(report_file, 'w', encoding='utf-8') as f:
            f.write("="*70 + "\n")
            f.write("LOGIC ANALYZER ANALYSIS REPORT\n")
            f.write("="*70 + "\n")
            f.write(f"Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
            f.write(f"Source File: {self.csv_file}\n")
            f.write(f"Total Samples: {len(self.df)}\n")
            f.write(f"Duration: {(self.df['Time_s'].max() - self.df['Time_s'].min())*1000:.3f} ms\n")
            f.write(f"Sample Rate: 8 MHz\n")
            f.write("="*70 + "\n\n")
            
            f.write("CHANNEL ANALYSIS SUMMARY\n")
            f.write("-"*70 + "\n")
            
            for ch_name, result in self.results.items():
                f.write(f"\n{ch_name.upper()}\n")
                f.write(f"  Signal Type      : {result['signal_type']}\n")
                f.write(f"  Frequency        : {result['frequency_avg_hz']:.2f} Hz ± {result['frequency_std_hz']:.2f} Hz\n")
                f.write(f"  Duty Cycle       : {result['duty_cycle_percent']:.2f}%\n")
                f.write(f"  Period (avg)     : {result['period_avg_us']:.3f} µs ± {result['period_std_us']:.3f} µs\n")
                f.write(f"  Total Edges      : {result['total_edges']}\n")
                f.write(f"  HIGH count       : {result['high_count']} samples\n")
                f.write(f"  LOW count        : {result['low_count']} samples\n")
                
            f.write("\n" + "="*70 + "\nEND OF REPORT\n" + "="*70 + "\n")
        print(f"📄 Report saved: {report_file}")
        
    def save_summary_csv(self):
        if not self.results: return
        summary_file = os.path.join(self.archive_folder, 'summary.csv')
        pd.DataFrame(self.results).T.to_csv(summary_file)
        print(f"📑 Summary CSV saved: {summary_file}")
        
    def save_metadata_json(self):
        metadata = {
            'timestamp': datetime.now().isoformat(),
            'source_file': self.csv_file,
            'total_samples': len(self.df),
            'duration_ms': (self.df['Time_s'].max() - self.df['Time_s'].min()) * 1000,
            'sample_rate_mhz': 8,
            'channels_analyzed': list(self.results.keys()),
            'results': self.results
        }
        json_file = os.path.join(self.archive_folder, 'metadata.json')
        with open(json_file, 'w') as f:
            json.dump(metadata, f, indent=2)
        print(f"📋 Metadata JSON saved: {json_file}")
        
    def run_full_analysis(self, channels=None):
        if channels is None:
            channels = [f'logic.{i}' for i in range(8)]
            
        print("\n🔍 Analyzing channels...")
        for ch in channels:
            idx = int(ch.split('.')[1]) if '.' in ch else 0
            result = self.analyze_channel(idx, ch)
            if result:
                print(f"  ✓ {ch}: {result['frequency_avg_hz']:.1f} Hz, {result['duty_cycle_percent']:.1f}% duty")
            else:
                print(f"  ⚪ {ch}: (idle or not present in CSV)")
                
        print("\n📊 Generating plots and reports...")
        self.plot_all_channels(channels)
        self.generate_report()
        self.save_summary_csv()
        self.save_metadata_json()
        
        print(f"\n✅ Archive complete! All files saved in: {self.archive_folder}/")
        print("-"*70)

# ... (kode class LogicAnalyzerArchiver sama seperti sebelumnya) ...

if __name__ == "__main__":
    import argparse
    import sys
    import os
    
    # Biar script bisa jalan dari mana saja (root atau software/)
    parser = argparse.ArgumentParser(description="Logic Analyzer Auto-Report Generator")
    parser.add_argument("csv_file", nargs="?", 
                        default=os.path.join("software", "examples", "sample_capture.csv"),
                        help="Path to CSV file (default: software/examples/sample_capture.csv)")
    args = parser.parse_args()
    
    # Cek apakah file ada
    if not os.path.exists(args.csv_file):
        print(f"❌ Error: File '{args.csv_file}' tidak ditemukan!")
        print("💡 Gunakan: python software/analyze_la_archive.py <path_ke_file.csv>")
        sys.exit(1)
        
    print(f"📂 Processing: {args.csv_file}")
    analyzer = LogicAnalyzerArchiver(args.csv_file)
    analyzer.run_full_analysis()