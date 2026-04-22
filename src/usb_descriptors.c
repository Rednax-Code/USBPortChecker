#include "tusb.h"

//
// Anti-Detection settings.
//
// Set USB IDs strings and numbers, to avoid possible detections.
// Remember that you can cusotmize FAKE_DISK_BLOCK_NUM as well
// for the same reason. Also DISK_LABEL in ramdisk.h can be changed.
//
// You can see here for inspiration: https://the-sz.com/products/usbid/
//
// Example:
//             0x0951 0x16D5    VENDORID_STR: Kingston   PRODUCTID_STR: DataTraveler
//
#define USB_VENDORID 0x0951               // This override the Pi Pico default 0x2E8A
#define USB_PRODUCTID 0x16D5              // This override the Pi Pico default 0x000A
#define USB_DESCRIPTOR "DataTraveler"     // This override the Pi Pico default "Pico"
#define USB_MANUF "Kingston"              // This override the Pi Pico default "Raspberry Pi"
#define USB_SERIAL "123456789A"           // This override the Pi Pico default. Disabled by default.
#define USB_VENDORID_STR "Kingston"       // Up to 8 chars
#define USB_PRODUCTID_STR "DataTraveler"  // Up to 16 chars
#define USB_VERSION_STR "1.0"             // Up to 4 chars

tusb_desc_device_t const desc_device = {
	.bLength            = sizeof(tusb_desc_device_t),
	.bDescriptorType    = TUSB_DESC_DEVICE,
	.bcdUSB             = 0x0200,             // USB 2.0
	.bDeviceClass       = 0x00,               // Defined in interface
	.bDeviceSubClass    = 0x00,
	.bDeviceProtocol    = 0x00,
	.bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

	.idVendor           = USB_VENDORID,       // <--- YOUR VENDOR ID
	.idProduct          = USB_PRODUCTID,      // <--- YOUR PRODUCT ID
	.bcdDevice          = 0x0100,
	
	.iManufacturer      = 0x01,               // Index of Manufacturer string
	.iProduct           = 0x02,               // Index of Product string
	.iSerialNumber      = 0x03,               // Index of Serial string
	.bNumConfigurations = 0x01
};

// This callback is REQUIRED by TinyUSB to send the descriptor above
uint8_t const * tud_descriptor_device_cb(void) {
	return (uint8_t const *) &desc_device;
}

// Array of strings: [0] is Language ID, [1] is Manufacturer, [2] is Product
char const* string_desc_arr [] = {
	(const char[]) { 0x09, 0x04 }, // 0: Supported language is English (0x0409)
	USB_MANUF,                     // 1: Manufacturer String
	USB_DESCRIPTOR,                // 2: Product Descriptor String
	"123456"                       // 3: Serial Number
};

static uint16_t _desc_str[32];

// This callback handles all string requests from the PC
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
	(void) langid;
	uint8_t chr_count;

	if (index == 0) {
		memcpy(&_desc_str[1], string_desc_arr[0], 2);
		chr_count = 1;
	} else {
		// Convert ASCII string to UTF-16
		if ( !(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) ) return NULL;
		const char* str = string_desc_arr[index];
		chr_count = strlen(str);
		if ( chr_count > 31 ) chr_count = 31;
		for(uint8_t i=0; i<chr_count; i++) _desc_str[1+i] = str[i];
	}

	// first byte is length (including header), second byte is string type (0x03)
	_desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);
	return _desc_str;
}


// Define interface indexes
enum {
    ITF_NUM_MSC,
    ITF_NUM_TOTAL
};

// Total length is config header + MSC interface + 2 endpoints
#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_MSC_DESC_LEN)

// Endpoint addresses
#define EPNUM_MSC_OUT     0x01
#define EPNUM_MSC_IN      0x81

uint8_t const desc_configuration[] = {
    // Config number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),

    // Interface number, string index, EP Out & EP In address, EP size
    TUD_MSC_DESCRIPTOR(ITF_NUM_MSC, 0, EPNUM_MSC_OUT, EPNUM_MSC_IN, 64)
};

// Callback for Configuration Descriptor
uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
    (void) index; // Only one configuration supported
    return desc_configuration;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
    (void) instance; (void) report_id; (void) report_type; (void) buffer; (void) reqlen;
    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
    (void) instance; (void) report_id; (void) report_type; (void) buffer; (void) bufsize;
}

// Invoked when received GET HID REPORT DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance) {
    (void) instance;
    // For now, return NULL or a dummy report if you aren't using HID yet. 
    // If you plan to send keystrokes later, you'll need a real HID report map here.
    return NULL; 
}