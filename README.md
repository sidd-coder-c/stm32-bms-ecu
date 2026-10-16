# stm32-bms-ecu

Battery monitor ECU firmware: STM32 + FreeRTOS + CAN. Cell voltage/temperature/current monitoring with fault detection.

**Status:** Day 3 of 30, in progress.
**Platform:** Wokwi simulation only (STM32 Blue Pill). No physical hardware.

## Docs
- Spec: docs/spec.md

## Progress
- Day 2: single-channel ADC read. Wokwi: https://wokwi.com/projects/477868684479712257
- Day 3: six-channel ADC read. Code in `firmware/`