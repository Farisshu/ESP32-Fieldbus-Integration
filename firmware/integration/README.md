# Integration Tests 🔗

Multi-node communication tests and integration scenarios for ESP32-based systems.

## Overview

This directory contains integration tests that verify communication between multiple devices or nodes. These tests go beyond unit testing to validate complete system behavior.

## Structure

```
integration/
├── can_two_nodes/     # Two-node CAN bus communication test
└── README.md          # This file
```

## Projects

### CAN Two Nodes (`can_two_nodes/`)

A dual-node CAN bus communication system using MCP2515 controllers.

**Features:**
- Configurable node roles (TX/RX)
- 500 kbps CAN bus communication
- Message transmission and reception verification
- Error handling and status reporting

**Hardware Required:**
- 2x ESP32 boards
- 2x MCP2515 CAN modules
- CAN bus wiring (CAN_H, CAN_L, termination resistors)

**Setup:**
```bash
cd firmware/integration/can_two_nodes
pio run --target upload  # Upload to both nodes
pio device monitor       # Monitor serial output
```

**Configuration:**
Edit `src/main.cpp` to set node role:
```cpp
#define NODE_ROLE RX  // or TX for transmitter node
```

## Integration Test Workflow

1. **Individual Node Testing**: Verify each node works independently
2. **Pairwise Testing**: Test communication between two nodes
3. **Network Testing**: Expand to multi-node networks
4. **Stress Testing**: Validate under load and error conditions

## Best Practices

- Document expected behavior for each test scenario
- Include pass/fail criteria in test descriptions
- Log all communication events for debugging
- Use consistent baud rates across all nodes
- Implement timeout handling for robustness

## Common Issues

| Issue | Cause | Solution |
|-------|-------|----------|
| No CAN messages | Missing termination | Add 120Ω resistor across CAN_H/L |
| CRC errors | Wiring issues | Check cable connections |
| Node not responding | Wrong configuration | Verify NODE_ROLE setting |

---

*For more information, see the main [firmware README](../README.md)*
