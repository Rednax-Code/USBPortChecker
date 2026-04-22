/*
  USBvalve

  RAMDISK file written by Cesare Pizzi
*/

/*
   The MIT License (MIT)

   Copyright (c) 2019 Ha Thach for Adafruit Industries

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#pragma once

#include <pico/stdlib.h>
#include "config.h"

//
// The filesystem contains 3 files at specific blocks (see also USBvalve.ino)
//    AUTORUN.INF
//    README.TXT
//    System Volume Information
//
// Files information are saved at Block 7
//

// Keep 11 chars
#define DISK_LABEL 'M', 'y', 'D', 'r', 'i', 'v', 'e', ' ', ' ', ' ', ' '

// Do not change this here, it is just for reference
#define README_CONTENTS \
  "...nuke the entire site from orbit. It's the only way to be sure."

// Do not change this here, it is just for reference
#define AUTORUN_CONTENTS \
  "[autorun]\r\nopen=calc.exe\r\nicon=icon.ico\r\n"

extern uint8_t msc_disk[DISK_BLOCK_NUM][DISK_BLOCK_SIZE];

