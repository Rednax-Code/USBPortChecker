#pragma once

#define FAKE_DISK_BLOCK_NUM 0x800
#define DISK_BLOCK_NUM 0x150
#define DISK_BLOCK_SIZE 0x200
#define BLOCK_AUTORUN 102       // Block where Autorun.inf file is saved
#define BLOCK_README 100        // Block where README.txt file is saved
#define MAX_DUMP_BYTES 16       // Used by the dump of the debug facility: do not increase this too much