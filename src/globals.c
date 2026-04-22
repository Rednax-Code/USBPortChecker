#include "globals.h"

volatile bool filesystem_is_valid = false;
volatile bool readme = false;
volatile bool written = false;
volatile bool autorun = false;
volatile bool deleted = false;
volatile bool written_reported = false;
volatile bool deleted_reported = false;
volatile bool hid_sent = false;
volatile bool hid_reported = false;