#include <stdio.h>

void hexDump(uint8_t* data, size_t size) {
	char asciitab[17];
	asciitab[16] = '\0';

	for (size_t i = 0; i < size; ++i) {

		printf("%02X ", data[i]);

		if ((data)[i] >= ' ' && (data)[i] <= '~') {
			asciitab[i % 16] = (data)[i];
		} else {
			asciitab[i % 16] = '.';
		}

		if ((i + 1) % 8 == 0 || i + 1 == size) {
			printf("  ");
			if ((i + 1) % 16 == 0) {
				printf("| %s\n", asciitab);
			} else if (i + 1 == size) {
                asciitab[(i + 1) % 16] = '\0';

                // Pad the hex area so the ASCII column stays aligned
                for (size_t j = (i + 1) % 16; j < 16; ++j) {
                    printf("   "); // Space for the missing hex numbers
                    if ((j + 1) % 8 == 0) printf("  "); // Space for the 8-byte gap
                }
                
                printf("| %s\n", asciitab);
			}
		}
	}
	fflush(stdout);
}