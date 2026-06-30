# Day 07: Debugging, Simulation, and Professional Practice

## Tujuan Pembelajaran

Setelah menyelesaikan hari ini, Anda akan mampu:
- Menerapkan systematic debugging methodology
- Menggunakan debugging tools secara efektif
- Membaca dan memahami legacy code
- Berkomunikasi profesional dalam tim engineering
- Menyusun daily report yang efektif
- Memahami workplace culture di perusahaan teknologi

---

## Konsep Dasar

### Mengapa Debugging Skill Penting?

Dalam industri:
- 30% menulis kode baru
- 70% debugging, testing, maintenance

> **Engineer Mindset:** "Debugging is understanding why the system doesn't work as expected."

---

## Systematic Debugging Methodology

### The Scientific Method:

```
1. Observe the problem
2. Form hypothesis (what might be wrong)
3. Design experiment to test
4. Run experiment, collect data
5. Analyze results
6. Accept/reject hypothesis
7. Repeat until root cause found
```

### Debugging Workflow:

```
Problem Reported
    ↓
Reproduce the Issue
    ↓
Isolate the Problem (which module/function?)
    ↓
Identify Root Cause
    ↓
Fix and Verify
    ↓
Document and Prevent
```

---

## Common Debugging Scenarios

### Scenario 1: MCU Not Starting

**Symptoms:** No UART output, LEDs not blinking

**Checklist:**
- [ ] Power supply OK? (VDD = 3.3V?)
- [ ] Reset pin pulled high?
- [ ] Clock configured correctly?
- [ ] Boot pins in correct position?
- [ ] Try re-flashing firmware

---

### Scenario 2: Random Resets

**Possible Causes:**
1. Brown-out reset (voltage drop)
2. Watchdog timeout
3. Stack overflow
4. Hard fault exception

**Debug Code:**
```c
void Debug_ResetCause(void) {
    uint32_t cause = RCC->CSR;
    
    if (cause & RCC_CSR_BORRSTF) 
        printf("Brown-out reset\n");
    if (cause & RCC_CSR_WDGRSTF) 
        printf("Watchdog reset\n");
    if (cause & RCC_CSR_SFTRSTF) 
        printf("Software reset\n");
    
    RCC->CSR |= RCC_CSR_RMVF; // Clear flags
}
```

---

### Scenario 3: Communication Failures

**I2C Debug Checklist:**
- [ ] Pull-up resistors present? (4.7k typical)
- [ ] Correct pins (check alternate functions)?
- [ ] Address correct? (7-bit vs 8-bit)
- [ ] Capture with logic analyzer

**UART Debug Checklist:**
- [ ] Baud rate matches exactly?
- [ ] TX/RX crossed correctly?
- [ ] Ground connection solid?

---

## Reading Legacy Code

### Strategy:

```
Step 1: High-Level Overview
  - Read README, documentation
  - Understand project structure

Step 2: Follow Execution Flow
  - Find main() entry point
  - Trace initialization sequence

Step 3: Understand Key Functions
  - Read function names/comments
  - Check call graphs

Step 4: Dive Into Details
  - Critical algorithms
  - Data structures
```

### Code Reading Questions:

```c
/* When reading a function, ask: */

// 1. What is the purpose?
// 2. What are the inputs/outputs?
// 3. What dependencies exist?
// 4. What are the edge cases?
// 5. What assumptions are made?
```

---

## Asking Questions Effectively

### Before Asking:

- [ ] Searched existing documentation?
- [ ] Checked error messages carefully?
- [ ] Tried to isolate the problem?
- [ ] Can reproduce consistently?
- [ ] Noted what I've already tried?

### Good Question Format:

```
Goal: [What you're trying to achieve]

What I did:
1. [Step 1]
2. [Step 2]

Expected: [What should happen]
Actual: [What actually happens]

What I've tried:
- [Attempt 1]
- [Attempt 2]

Questions:
- [Specific question 1]
- [Specific question 2]

Environment:
- [MCU, IDE, version]
```

---

## Daily Report Template

```
Date: YYYY-MM-DD
Engineer: [Your Name]

## Today's Progress
- [Task 1]: Completed
- [Task 2]: In progress (80%)
- [Task 3]: Blocked by [reason]

## Issues Encountered
1. [Issue description]
   - Root cause: [analysis]
   - Resolution: [fix applied]

## Tomorrow's Plan
- [ ] Complete Task 2
- [ ] Start Task 4
- [ ] Review PR #XX

## Blockers/Help Needed
- Need clarification on [topic]
- Waiting for [component/approval]
```

---

## Professional Attitude

### Workplace Best Practices:

1. **Communication**
   - Update team on progress daily
   - Ask for help early when stuck
   - Document decisions and rationale

2. **Code Quality**
   - Write code for others to read
   - Review your own code before submitting
   - Learn from code reviews

3. **Time Management**
   - Estimate tasks realistically
   - Communicate delays early
   - Prioritize based on impact

4. **Continuous Learning**
   - Read datasheets thoroughly
   - Stay updated with industry practices
   - Share knowledge with team

---

## Kesalahan yang Sering Terjadi

| Error | Better Approach |
|-------|-----------------|
| "It works on my machine" | Test in target environment |
| Changing multiple things at once | Change one variable at a time |
| Not documenting debug steps | Keep debug log |
| Assuming without measuring | Use scope/multimeter |
| Giving up too early | Take break, then retry |

---

## Tips Engineer Profesional

1. **Keep a Debug Journal** - Document every bug and solution
2. **Rubber Duck Debugging** - Explain problem out loud
3. **Take Breaks** - Fresh eyes find bugs faster
4. **Celebrate Fixes** - Every bug fixed is learning gained
5. **Share Knowledge** - Write post-mortems for major issues

---

## Ringkasan

| Skill | Key Point |
|-------|-----------|
| Debugging | Scientific method, systematic approach |
| Tools | Know your scope, logic analyzer, debugger |
| Legacy Code | Read docs first, follow execution flow |
| Communication | Clear questions, daily updates |
| Professionalism | Document, collaborate, continuous learning |

---

## Referensi Belajar

1. "Debugging" - David J Agans
2. "Working Effectively with Legacy Code" - Michael Feathers
3. Embedded Artistry Blog - Debugging guides
4. Jacob Beningo YouTube - Embedded best practices

---

> **"The best engineer is not the one who never makes mistakes, but the one who learns fastest from them."**

Welcome to the professional world of embedded systems engineering!
