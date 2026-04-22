#pragma once

#define FAKE_DISK_BLOCK_NUM 0x800
#define DISK_BLOCK_NUM 0x150
#define DISK_BLOCK_SIZE 0x200
#define BLOCK_AUTORUN 102       // Block where Autorun.inf file is saved
#define BLOCK_README 100        // Block where README.txt file is saved
#define MAX_DUMP_BYTES 16       // Used by the dump of the debug facility: do not increase this too much

// Vars for the Hashing algorithm
#define BYTES_TO_HASH 512 * 2   // Number of bytes of the RAM disk used in the check
#define BYTES_TO_HASH_OFFSET 7  // Starting sector of the check (FAT_DIRECTORY is 7)

// Define vars for OLED screen
#define I2C_PORT i2c0
#define I2C_PIN_SDA 4
#define I2C_PIN_SCL 5
#define I2C_ADDRESS 0x3C  // 0X3C+SA0 - 0x3C or 0x3D

// Use this to turn on/off the blinking light
//#define DEBUG