/*********************************************************************

 USBvalve -> USBPortChecker
 
 written by Rednax-Code, BUT!
 This project extensively reused code done by Cesare Pizzi and thereby in turn Adafruit and TinyUSB. 
 Please support them, not me!

*********************************************************************/

#include <pico/stdlib.h>
#include <pico/bootrom.h>

#include "ssd1306.h"
#include "tusb.h"
#include "device/usbd.h"
#include "hardware/i2c.h"
#include "xxhash.h"

#include "interface.h"
#include "globals.h"
#include "config.h"
#include "ramdisk.h"

// Burned hash of a valid file system
uint valid_hash = 3534146792;

#ifdef DEBUG
#include <pico/time.h>
// The light on board for debugging purposes
const uint LED_PIN = PICO_DEFAULT_LED_PIN;
#endif

// Setup Core0
void setup() {

	#ifdef DEBUG
	// Blinking light
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);
	#endif

	// Change all the USB Pico settings
	stdio_init_all();

	// Check consistency of RAM FS
	// Add 11 bytes to skip the DISK_LABEL from the hashing
	// The startup of the USB has been moved before initialization of the 
	// screen because sometimes it inserts some delay preventing
	// proper initialization of the mass device
    uint32_t computed_hash = XXH32(msc_disk + BYTES_TO_HASH_OFFSET + 11, BYTES_TO_HASH, 0);
    
	// Check file system
    if (computed_hash == valid_hash) {
        filesystem_is_valid = true;
	}

	// Screen Init
	i2c_init(I2C_PORT, 1000000);
	gpio_set_function(I2C_PIN_SDA, GPIO_FUNC_I2C);
	gpio_set_function(I2C_PIN_SCL, GPIO_FUNC_I2C);
	gpio_pull_up(I2C_PIN_SDA);
	gpio_pull_up(I2C_PIN_SCL);
}

void screen_boot(pico_ssd1306::SSD1306* display) {
	// Fix orientation
	display->setOrientation(0);

	// Clear display
	cls(display);

	// Now output the result of the check
	if (filesystem_is_valid) {
		printout(display, "[+] Filesystem: Check");
		tusb_init();
	} else {
		uint32_t computed_hash = XXH32(msc_disk + BYTES_TO_HASH_OFFSET + 11, BYTES_TO_HASH, 0);
		
		printout(display, "[!] Filesystem: Error");
		printout(display, "HASH MISMATCH!");

		#ifdef DEBUG
		// Panic blink
		while(true) {
			gpio_put(LED_PIN, 1); sleep_ms(50);
			gpio_put(LED_PIN, 0); sleep_ms(50);
		}
		#endif
	}
}


// Loop Core0
void loop(pico_ssd1306::SSD1306* display) {
	if (readme == true) {
		printout(display, "[!] README (R)");
		readme = false;
	}

	if (autorun == true) {
		printout(display, "[+] AUTORUN (R)");
		autorun = false;
	}

	if (deleted == true && deleted_reported == false) {
		printout(display, "[!] DELETING");
		deleted = false;
		deleted_reported = true;
	}

	if (written == true && written_reported == false) {
		printout(display, "[!] WRITING");
		written = false;
		written_reported = true;
	}

	if (hid_sent == true && hid_reported == false) {
		printout(display, "[!!] HID Sending data");
		hid_sent = false;
		hid_reported = true;
	}
}


int main() {

	setup();

	// Startup display
	pico_ssd1306::SSD1306 display = pico_ssd1306::SSD1306(I2C_PORT, I2C_ADDRESS, pico_ssd1306::Size::W128xH64);

	screen_boot(&display);

	#ifdef DEBUG
	static uint32_t last_blink_time = 0;
	uint32_t current_time;
	#endif

    while (true) {
		// Keep the USB stack alive
		tud_task();

		// Call the main loop
		loop(&display);

		#ifdef DEBUG
		// Blinker
		current_time = to_ms_since_boot(get_absolute_time());
    	
		if (current_time - last_blink_time >= 100) {
			last_blink_time = current_time;
			gpio_put(LED_PIN, !gpio_get(LED_PIN));
		}
		#endif
	}
}