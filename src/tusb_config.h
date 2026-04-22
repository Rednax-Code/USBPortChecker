#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

// --- USB Controller Configuration ---
// RP2040 uses controller 0
#ifndef CFG_TUSB_RHPORT0_MODE
#define CFG_TUSB_RHPORT0_MODE    OPT_MODE_DEVICE
#endif

#ifndef CFG_TUSB_OS
#define CFG_TUSB_OS              OPT_OS_PICO
#endif

// --- MSC Class Configuration ---
#define CFG_TUD_MSC              1

// This is the specific fix for your error.
// 512 bytes is the standard block size for FAT filesystems.
// 4096 (8 blocks) often provides better performance.
#define CFG_TUD_MSC_EP_BUFSIZE   512 

// --- HID Class Configuration (if you are using it for your 'twist') ---
#define CFG_TUD_HID              0
#define CFG_TUD_HID_EP_BUFSIZE   64

#endif