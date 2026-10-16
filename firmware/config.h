#pragma once

#define NUM_CH            6
#define ADC_MAX_COUNTS    4095     // 12-bit
#define ADC_VREF_MV       3300

// 3.3 V at the ADC pin represents 5.0 V at the cell.
// A pot stands in for the real resistor divider in simulation.
#define CELL_DIVIDER_NUM  5000
#define CELL_DIVIDER_DEN  3300

// Current: 0..3.3 V at the pin maps to -50..+50 A
#define I_MIN_MA          (-50000L)
#define I_MAX_MA          (50000L)