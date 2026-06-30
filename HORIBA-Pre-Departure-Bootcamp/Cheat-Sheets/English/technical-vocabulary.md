# Technical Vocabulary (English)

## Essential English Terms for Embedded Engineers

---

## Core Embedded Concepts

| Term | Definition | Example Usage |
|------|------------|---------------|
| **Embedded System** | A computer system designed for specific functions within a larger system | "The embedded system controls the motor speed." |
| **Microcontroller (MCU)** | A compact integrated circuit with processor, memory, and peripherals | "We're using an STM32 microcontroller." |
| **Firmware** | Permanent software programmed into read-only memory | "The firmware needs to be updated." |
| **Real-time** | Processing data immediately as it arrives | "This is a real-time control system." |
| **Interrupt** | Signal that causes CPU to pause current task and execute ISR | "The timer interrupt fires every 1ms." |
| **Peripheral** | Hardware component connected to the MCU | "Enable the UART peripheral clock." |
| **Register** | Small storage location in CPU or peripheral | "Write 1 to the control register." |
| **GPIO** | General Purpose Input/Output pin | "Configure PA5 as GPIO output." |
| **ADC** | Analog-to-Digital Converter | "The ADC reads sensor voltage." |
| **DAC** | Digital-to-Analog Converter | "Use DAC to generate analog signal." |
| **PWM** | Pulse Width Modulation | "Control LED brightness with PWM." |
| **DMA** | Direct Memory Access | "Use DMA for efficient data transfer." |
| **ISR** | Interrupt Service Routine | "Keep the ISR short and fast." |

---

## Communication Protocols

| Term | Definition | Example Usage |
|------|------------|---------------|
| **UART** | Universal Asynchronous Receiver-Transmitter | "Connect via UART at 115200 baud." |
| **SPI** | Serial Peripheral Interface | "The flash memory uses SPI interface." |
| **I2C** | Inter-Integrated Circuit | "Sensor communicates over I2C bus." |
| **CAN** | Controller Area Network | "Automotive systems use CAN bus." |
| **USB** | Universal Serial Bus | "Device connects via USB CDC." |
| **Ethernet** | Wired network protocol | "Add Ethernet connectivity." |
| **Baud Rate** | Symbol rate for serial communication | "Set baud rate to 9600." |
| **Bitrate** | Number of bits transmitted per second | "SPI bitrate is 10 Mbps." |
| **Protocol** | Set of rules for communication | "Implement the MODBUS protocol." |
| **Packet** | Unit of data transmitted | "Parse the incoming packet." |
| **Checksum** | Error-detection code | "Verify the checksum." |
| **CRC** | Cyclic Redundancy Check | "Calculate CRC-16 for data integrity." |

---

## Software Development

| Term | Definition | Example Usage |
|------|------------|---------------|
| **Compiler** | Program that translates source code to machine code | "Compile with optimization -O2." |
| **Debugger** | Tool for finding and fixing bugs | "Use the debugger to step through code." |
| **Linker** | Program that combines object files | "The linker creates the final binary." |
| **Bootloader** | Program that loads the main application | "Jump to bootloader for update." |
| **Toolchain** | Suite of development tools | "Install the ARM toolchain." |
| **IDE** | Integrated Development Environment | "Use VS Code with PlatformIO IDE." |
| **Version Control** | System for tracking code changes | "Commit changes to version control." |
| **Repository** | Storage location for code | "Push code to the repository." |
| **Branch** | Parallel version of code | "Create a feature branch." |
| **Merge** | Combine changes from branches | "Merge the pull request." |
| **Pull Request** | Request to merge code changes | "Submit a pull request for review." |
| **Code Review** | Examination of code by peers | "Request code review from team." |
| **CI/CD** | Continuous Integration/Continuous Deployment | "Set up CI/CD pipeline." |

---

## Debugging & Testing

| Term | Definition | Example Usage |
|------|------------|---------------|
| **Breakpoint** | Point where execution pauses | "Set breakpoint at main()." |
| **Watchpoint** | Breakpoint triggered by variable change | "Add watchpoint on counter." |
| **Stack Trace** | List of active function calls | "Check stack trace for error." |
| **Core Dump** | Memory snapshot at crash | "Analyze the core dump." |
| **Unit Test** | Test for individual component | "Write unit tests for driver." |
| **Integration Test** | Test for combined components | "Run integration tests." |
| **Regression Test** | Test to ensure no new bugs | "Perform regression testing." |
| **Hard Fault** | Critical error causing reset | "System enters HardFault handler." |
| **Watchdog** | Timer that resets system if not fed | "Enable watchdog for safety." |
| **Logic Analyzer** | Tool to capture digital signals | "Probe with logic analyzer." |
| **Oscilloscope** | Tool to visualize analog signals | "Check waveform on oscilloscope." |
| **JTAG/SWD** | Debug interface protocols | "Connect via SWD debugger." |

---

## Hardware & Electronics

| Term | Definition | Example Usage |
|------|------------|---------------|
| **PCB** | Printed Circuit Board | "Design the PCB layout." |
| **Schematic** | Circuit diagram | "Review the schematic." |
| **Datasheet** | Technical document for component | "Read the datasheet carefully." |
| **Reference Design** | Example circuit from manufacturer | "Follow the reference design." |
| **Decoupling Capacitor** | Capacitor that filters noise | "Add decoupling capacitors near VDD." |
| **Pull-up/Pull-down** | Resistor that sets default state | "Enable internal pull-up resistor." |
| **Level Shifter** | Circuit that converts voltage levels | "Use level shifter for 5V logic." |
| **Voltage Regulator** | Device that maintains constant voltage | "LDO regulator provides 3.3V." |
| **Crystal Oscillator** | Component that generates clock | "16 MHz crystal for system clock." |
| **Reset Circuit** | Circuit that initializes MCU | "Check reset circuit timing." |
| **Power Supply** | Source of electrical power | "Stable power supply is critical." |
| **Ground Plane** | Large copper area for ground | "Use solid ground plane." |
| **Signal Integrity** | Quality of electrical signal | "Ensure good signal integrity." |

---

## Memory Types

| Term | Definition | Example Usage |
|------|------------|---------------|
| **Flash** | Non-volatile program memory | "Code is stored in Flash." |
| **SRAM** | Volatile data memory | "Variables stored in SRAM." |
| **EEPROM** | Electrically erasable non-volatile memory | "Save settings to EEPROM." |
| **DRAM** | Dynamic RAM (external) | "External DRAM for large buffers." |
| **Cache** | High-speed memory for frequently used data | "Enable instruction cache." |
| **Heap** | Memory for dynamic allocation | "Avoid malloc on heap." |
| **Stack** | Memory for local variables and function calls | "Monitor stack usage." |
| **Memory Map** | Layout of memory addresses | "Check memory map in linker script." |

---

## Common Documentation Terms

| Term | Definition | Example Usage |
|------|------------|---------------|
| **Specification** | Detailed requirements document | "Follow the specification." |
| **Requirement** | Something the system must do | "Meet all functional requirements." |
| **Implementation** | Actual code/solution | "Review the implementation." |
| **Interface** | Boundary between components | "Define clear interfaces." |
| **API** | Application Programming Interface | "Use the HAL API." |
| **Driver** | Software that controls hardware | "Write GPIO driver." |
| **Middleware** | Software layer between OS and application | "Add middleware for networking." |
| **Framework** | Structured foundation for development | "Use STM32Cube framework." |
| **Library** | Reusable code collection | "Link against standard library." |
| **Module** | Self-contained code unit | "Create sensor module." |
| **Component** | Individual part of system | "Test each component separately." |
| **Subsystem** | Major functional division | "Power management subsystem." |

---

## Problem-Solving Vocabulary

| Term | Definition | Example Usage |
|------|------------|---------------|
| **Issue** | Problem or bug | "Report the issue on GitHub." |
| **Bug** | Error in software | "Fix the timing bug." |
| **Root Cause** | Fundamental reason for problem | "Find the root cause." |
| **Workaround** | Temporary solution | "Implement workaround until fix." |
| **Patch** | Small code fix | "Apply the security patch." |
| **Hotfix** | Urgent production fix | "Deploy hotfix immediately." |
| **Debug** | Process of finding bugs | "Debug the communication issue." |
| **Troubleshoot** | Systematic problem solving | "Troubleshoot the hardware." |
| **Diagnose** | Identify the problem | "Diagnose the failure mode." |
| **Resolve** | Fix the problem | "Resolve the ticket." |
| **Mitigate** | Reduce impact of problem | "Mitigate the risk." |
| **Prevent** | Stop problem from occurring | "Prevent future occurrences." |

---

## Professional Communication

| Term | Definition | Example Usage |
|------|------------|---------------|
| **Deadline** | Due date for task | "Meet the project deadline." |
| **Milestone** | Significant point in project | "Reach the first milestone." |
| **Deliverable** | Expected output | "Submit all deliverables." |
| **Scope** | Boundaries of project | "Define project scope." |
| **Timeline** | Schedule of activities | "Update the timeline." |
| **Status Report** | Progress update | "Send weekly status report." |
| **Blocker** | Something preventing progress | "Remove the blocker." |
| **Dependency** | Something task relies on | "Identify dependencies." |
| **Risk** | Potential future problem | "Assess project risks." |
| **Assumption** | Something taken for granted | "Document all assumptions." |
| **Constraint** | Limitation on project | "Work within constraints." |
| **Trade-off** | Compromise between options | "Evaluate trade-offs." |

---

## Acronyms Quick Reference

| Acronym | Full Form |
|---------|-----------|
| MCU | Microcontroller Unit |
| MPU | Microprocessor Unit |
| FPU | Floating Point Unit |
| ALU | Arithmetic Logic Unit |
| DMA | Direct Memory Access |
| ADC | Analog-to-Digital Converter |
| DAC | Digital-to-Analog Converter |
| PWM | Pulse Width Modulation |
| UART | Universal Asynchronous Receiver-Transmitter |
| SPI | Serial Peripheral Interface |
| I2C | Inter-Integrated Circuit |
| CAN | Controller Area Network |
| USB | Universal Serial Bus |
| GPIO | General Purpose Input/Output |
| ISR | Interrupt Service Routine |
| NVIC | Nested Vectored Interrupt Controller |
| PLL | Phase Locked Loop |
| CRC | Cyclic Redundancy Check |
| FIFO | First In First Out |
| LIFO | Last In First Out |
| PCB | Printed Circuit Board |
| BOM | Bill of Materials |
| ESD | Electrostatic Discharge |
| EMC | Electromagnetic Compatibility |
| EMI | Electromagnetic Interference |

---

*Last Updated: [Tanggal]*  
*For more resources, see References/recommended-references.md*
