# BMS ECU: Specification (v0.1, Day 1)

Status: draft. Items marked **[CONFIRM]** are placeholder values to verify against a datasheet before Day 9.

## 1. Purpose
Firmware for a battery monitor ECU. It measures cell voltages, pack temperature and pack current, detects unsafe conditions, and reports pack state over CAN.

Runs in **simulation only** (Wokwi, STM32 Blue Pill / STM32F103C8T6). No physical hardware, no real cells.

## 2. Scope

**In scope (v1)**
- Sample 4 cell voltages, 1 temperature, 1 pack current
- Detect 4 faults: over-voltage, under-voltage, over-temperature, over-current
- Fault state machine with debounce and hysteresis
- Transmit pack state and fault flags over CAN at 10 Hz
- Host-side unit tests for fault logic and CAN packing

**Out of scope (v1)**
- State-of-charge (SOC) estimation
- Cell balancing
- Real hardware, real cells, contactor control
- AUTOSAR / ISO 26262 compliance (vocabulary only, no claims)

## 3. Inputs (simulated)

| Signal | Count | Source in Wokwi | Range (physical) | Notes |
|---|---|---|---|---|
| Cell voltage | 4 | 4 potentiometers on ADC | 0 to 5.0 V **[CONFIRM]** | Scaled from ADC counts to mV |
| Pack temperature | 1 | NTC thermistor model | -20 to 100 C **[CONFIRM]** | Beta equation |
| Pack current | 1 | 1 potentiometer on ADC | -50 to +50 A **[CONFIRM]** | Sign = charge/discharge |

ADC: 12-bit, reference voltage 3.3 V. Sampling period: 100 ms.

## 4. Fault definitions

| Fault | Trigger | Clear | Debounce | Source of threshold |
|---|---|---|---|---|
| Over-voltage (OV) | any cell > 4.20 V **[CONFIRM]** | all cells < 4.15 V | 3 samples | Typical Li-ion limit, verify with datasheet |
| Under-voltage (UV) | any cell < 2.80 V **[CONFIRM]** | all cells > 2.90 V | 3 samples | Typical Li-ion limit, verify with datasheet |
| Over-temperature (OT) | temp > 60 C **[CONFIRM]** | temp < 55 C | 3 samples | Placeholder |
| Over-current (OC) | abs(current) > 30 A **[CONFIRM]** | abs(current) < 28 A | 3 samples | Placeholder |

States: `NORMAL` -> `WARNING` (within 5% of a limit) -> `FAULT` (limit crossed and debounce satisfied).
Latching: **[DECIDE by Day 27]** whether faults latch until reset or clear automatically.

## 5. Software architecture (FreeRTOS)

| Task | Period | Priority | Job |
|---|---|---|---|
| Sampling | 100 ms | high | Read ADC, scale, filter, push to queue |
| Fault check | on queue receive | medium | Run fault logic, update state and flags |
| CAN transmit | 100 ms | medium | Pack state into frames and transmit |

Communication: queue from Sampling to Fault check. Shared fault flags protected by mutex or event group (**[DECIDE] Day 18**).

## 6. CAN output (draft, finalized on Day 21 in `bms.dbc`)

| ID | Name | Rate | Content |
|---|---|---|---|
| 0x100 | BMS_Cells | 10 Hz | Cell 1-4 voltage, 16-bit each, mV |
| 0x101 | BMS_Pack | 10 Hz | Pack voltage (mV), pack current (0.1 A), temperature (0.1 C) |
| 0x102 | BMS_Status | 10 Hz | State (NORMAL/WARNING/FAULT), fault flags (OV/UV/OT/OC), alive counter |

Bit rate: 500 kbit/s **[CONFIRM]**. Byte order: little-endian.
If Wokwi cannot simulate CAN, frames are printed over UART in candump format and decoded on the PC.

## 7. Verification
- Host-side unit tests (gcc) for fault logic and CAN packing
- Fault-injection table: injected condition vs expected flag and CAN output (Day 26)
- Demo recording of Wokwi inputs changing and faults tripping (Day 29)

## 8. Definition of done (v1.0, Day 30)
- All 4 faults detected, debounced and reported over CAN
- Unit tests passing
- README with architecture, CAN map, how to run, known limitations
- Demo linked in README, repo tagged `v1.0`

## 9. Known limitations
- Simulation only: no real sensor noise, no real battery dynamics
- Thresholds are generic Li-ion placeholders, not tuned to a specific cell
- No functional-safety claims
