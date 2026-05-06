# 🧰 Hardware Validation Checklist (Archived)

**Status:** ⚠️ ARCHIVED - For reference during hardware debugging only  
**Last Updated:** 2026-05-03  
**Author:** M. Faris A. G.

---

## Purpose

This checklist is **NOT** for daily use. Run these checks **ONLY** when experiencing:
- TX shows `TXABT=1` or `EFLG>0x00` consistently
- RX shows inconsistent ID decoding
- System exhibits random reboots or freezes
- SPI communication failures

---

## CAN Bus Physical Layer

### Termination & Wiring
- [ ] **120Ω termination resistor** installed at **BOTH ends** of CAN bus
- [ ] **CANH ↔ CANH** connection verified (no swap with CANL)
- [ ] **CANL ↔ CANL** connection verified (no swap with CANH)
- [ ] **Common GND** connected across all nodes (ESP32, MCP2515, TJA1050)
- [ ] Cable length ≤ 1 meter for prototyping (≤40m for 500kbps in production)

### TJA1050 Transceiver
- [ ] VCC = 5.0V ±0.25V (measure with multimeter)
- [ ] GND connected to ESP32 GND
- [ ] CANH/CANL pins not swapped
- [ ] No short between CANH and CANL

### MCP2515 Module
- [ ] VCC = 3.3V or 5V (per module spec)
- [ ] CS pin connected to GPIO 5
- [ ] SI/SO/SCK pins connected to SPI bus
- [ ] INT pin (if used) connected correctly

---

## SPI Signal Integrity

### Physical Connections
- [ ] SPI cable length **<15cm** for prototyping
- [ ] Level shifter (LLC) properly configured (3.3V ↔ 5V if needed)
- [ ] No loose connections on SPI header
- [ ] CS lines separate for MCP2515 (GPIO 5) and TFT (GPIO 17)

### Signal Quality
- [ ] SPI frequency ≤250kHz for stability (increase only after verification)
- [ ] SCK waveform clean on oscilloscope (no ringing/overshoot)
- [ ] MOSI/MISO signals stable during transactions
- [ ] No cross-talk between SPI lines

---

## Power Supply

### Voltage Levels
- [ ] ESP32 3.3V rail stable under load
- [ ] MCP2515 VCC within spec (3.3V or 5V per module)
- [ ] TJA1050 VCC = 5.0V
- [ ] TFT VCC = 3.3V or 5V (per module)
- [ ] All GND points common (measure resistance <1Ω)

### Current Consumption
- [ ] Total current draw <500mA (USB port limit)
- [ ] No voltage droop >0.2V during CAN transmission
- [ ] Decoupling capacitors present near each IC

---

## Debugging Flowchart

```
Problem: EFLG > 0x00 or TXABT=1
│
├─► Check 120Ω termination ──── Missing? → Add termination
│                               Present? ↓
├─► Measure CANH-CANL voltage ── Not ~2.5V? → Check transceiver power
│                                OK (~2.5V)? ↓
├─► Verify GND continuity ────── Open circuit? → Fix GND connection
│                                Continuity OK? ↓
├─► Check SPI frequency ──────── >250kHz? → Reduce to 250kHz
│                                ≤250kHz? ↓
├─► Inspect waveform (LA/Scope) ─ Noisy? → Shorten cables, add shielding
│                                  Clean? ↓
└─► Replace MCP2515 module ───── Clone module may be defective
```

---

## EFLG Register Reference

| Bit | Name | Description | Action |
|-----|------|-------------|--------|
| 0 | EWARN | Error Warning Limit reached | Monitor, usually recovers |
| 1 | RXWAR | Receive Error Warning | Check bus termination |
| 2 | TXWAR | Transmit Error Warning | Check TX line, termination |
| 3 | RXEP | Receive Error Passive | Serious RX fault |
| 4 | TXEP | Transmit Error Passive | Serious TX fault |
| 5 | TXBO | Bus-Off | Reset MCP2515 required |
| 6 | RXB0OVFL | RX Buffer 0 Overflow | Increase poll rate |
| 7 | Reserved | - | Ignore |

**Normal Prototyping:** EFLG = 0x05 (EWARN + TXWAR) is acceptable  
**Production Target:** EFLG = 0x00 consistently

---

## When to Use This Checklist

✅ **Run this checklist when:**
- Firmware reports consistent EFLG errors (>0x80)
- CAN frames not received despite correct ID filter
- System crashes during CAN transmission
- SPI read/write returns invalid data

❌ **Do NOT run this checklist for:**
- Normal development (use Serial monitor instead)
- Software bugs (check code logic first)
- UI display issues (check TFT wiring separately)

---

## Related Documentation

- [Test Procedures SOP](../test_procedures.md)
- [Firmware README](../../firmware/README.md)
- [Troubleshooting Guide](../../README.md#troubleshooting)

---

*This checklist is archived for hardware debugging reference. For normal operation, trust the firmware's EFLG monitoring and Serial output.*
