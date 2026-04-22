#include <string.h>

#include "globals.h"
#include "hexdump.h"
#include "config.h"
#include "ramdisk.h"

void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size) {
    (void) lun;
    *block_count = FAKE_DISK_BLOCK_NUM;
    *block_size  = DISK_BLOCK_SIZE;
}

// READY CHECK Callback
bool tud_msc_test_unit_ready_cb(uint8_t lun) {
    // Only tell the PC the drive is ready if the hash matched
    return filesystem_is_valid; 
}

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and
// return number of copied bytes (must be multiple of block size).
// This happens only for the "real" size of disk
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {

	// Check for README.TXT
	if (lba == BLOCK_README) {
		readme = true;
	}

	// Check for AUTORUN.INF
	if (lba == BLOCK_AUTORUN) {
		autorun = true;
	}

	// We are declaring a bigger size than what is actually allocated, so
	// this is protecting our memory integrity
	if (lba < DISK_BLOCK_NUM - 1) {
		uint8_t const* addr = msc_disk[lba];
		memcpy(buffer, addr, bufsize);
	}

	printf("Read LBA: %lu  Size: %lu\n", (unsigned long)lba, (unsigned long)bufsize);
    
	if (lba < DISK_BLOCK_NUM - 1) {
		hexDump(msc_disk[lba], MAX_DUMP_BYTES);
	}
	fflush(stdout);

	return bufsize;
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and
// return number of written bytes (must be multiple of block size).
// This happens only for the "real" size of disk
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {

	// Check for file deletion at Block 7
	// The first char of filename is replaced with 0xE5, we are going
	// to check for it
	if (lba == 7) {
		if (buffer[32] == 0xE5 || buffer[64] == 0xE5 || buffer[160] == 0xE5) {
			deleted = true;
		}
	}

	// This check for writing of space. The LBA > 10 is set to avoid some
	// false positives, in particular on Windows Systems
	if (lba > 10) {
		written = true;
	}

	// We are declaring a bigger size than what is actually allocated, so
	// this is protecting our memory integrity
	if (lba < DISK_BLOCK_NUM - 1) {
		// Writing buffer to "disk"
		uint8_t* addr = msc_disk[lba];
		memcpy(addr, buffer, bufsize);
	}

	printf("Read LBA: %lu  Size: %lu\n", (unsigned long)lba, (unsigned long)bufsize);

	if (lba < DISK_BLOCK_NUM - 1) {
		hexDump(msc_disk[lba], MAX_DUMP_BYTES);
	}
	fflush(stdout);

	return bufsize;
}

// Callback invoked when WRITE10 command is completed (status received and accepted by host).
// used to flush any pending cache.
void tud_msc_writable_callback(void) {
	// Nothing to do
}

// Invoked when received SCSI_INQUIRY command.
// Application fill vendor id, product id and revision with string up to 8, 16, 4 characters respectively
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {
    (void) lun;
    // These must match your "Anti-Detection" settings
    const char vid[] = "Kingston";
    const char pid[] = "DataTraveler";
    const char rev[] = "1.0";

    memcpy(vendor_id,  vid, strlen(vid));
    memcpy(product_id, pid, strlen(pid));
    memcpy(product_rev, rev, strlen(rev));
}

// Invoked when received Test Unit Ready command.
// return true allowing host to read/write this LUN
int32_t tud_msc_scsi_cb(uint8_t lun, const uint8_t* scsi_cmd, void* buffer, uint16_t bufsize) {
    // read10 and write10 are handled by their own callbacks, 
    // so we just return true for other generic SCSI commands.
    (void) lun; (void) scsi_cmd; (void) buffer; (void) bufsize;
    return true;
}

#ifdef BTN_EJECT
// Invoked when received Test Unit Ready command.
// return true allowing host to read/write this LUN e.g SD card inserted
bool msc_ready_callback(void) {
	// button not active --> medium ready
	return digitalRead(BTN_EJECT) != activeState;
}
#endif