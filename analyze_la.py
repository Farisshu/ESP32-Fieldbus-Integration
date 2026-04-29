import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# 1. Baca CSV
df = pd.read_csv('Data Logic Analizer Test1 ESP32U.csv')

# 2. Konversi waktu dari nanoseconds ke seconds
df['Time_s'] = df['nanoseconds'] / 1e9

# 3. Pilih channel yang mau dianalisis (misal D2 = kolom ke-4, index 3)
channel_name = 'logic.2'  # D2 = PWM
signal = df[channel_name]

# 4. Hitung Frekuensi (metode edge counting)
rising_edges = np.diff(signal) == 1  # Deteksi 0→1
freq_hz = rising_edges.sum() / (df['Time_s'].max() - df['Time_s'].min())
print(f"📊 {channel_name} Frequency: {freq_hz:.2f} Hz")

# 5. Hitung Duty Cycle
duty_cycle = signal.mean() * 100
print(f"⚡ Duty Cycle: {duty_cycle:.2f}%")

# 6. Plot Waveform (seperti osiloskop)
plt.figure(figsize=(10, 3))
plt.plot(df['Time_s']*1000, signal, drawstyle='steps-pre', linewidth=1)
plt.xlabel('Time (ms)')
plt.ylabel('Logic Level')
plt.title(f'{channel_name} - Frequency: {freq_hz:.2f} Hz, Duty: {duty_cycle:.2f}%')
plt.grid(True, alpha=0.3)
plt.ylim(-0.2, 1.2)
plt.tight_layout()
plt.savefig('waveform_D2.png', dpi=150)  # Simpan gambar
plt.show()

# 7. Export Summary ke CSV baru
summary = {
    'Channel': [channel_name],
    'Frequency_Hz': [freq_hz],
    'Duty_Cycle_%': [duty_cycle],
    'Total_Samples': [len(df)],
    'Duration_s': [df['Time_s'].max() - df['Time_s'].min()]
}
pd.DataFrame(summary).to_csv('summary_analysis.csv', index=False)
print("✅ Summary saved to summary_analysis.csv")