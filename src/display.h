#pragma once

#include "epd_driver.h"
#include "epd_highlevel.h"

#define WAVEFORM EPD_BUILTIN_WAVEFORM

EpdiyHighlevelState hl;

void initDisplay() {
    epd_init(EPD_LUT_1K);
    hl = epd_hl_init(WAVEFORM);

    // Default orientation is EPD_ROT_LANDSCAPE
    epd_set_rotation(EPD_ROT_LANDSCAPE);
    epd_poweron();
    epd_clear();
}