#pragma once

#include <pico/stdlib.h>

extern volatile bool filesystem_is_valid;
extern volatile bool readme;
extern volatile bool written;
extern volatile bool autorun;
extern volatile bool deleted;
extern volatile bool written_reported;
extern volatile bool deleted_reported;
extern volatile bool hid_sent;
extern volatile bool hid_reported;