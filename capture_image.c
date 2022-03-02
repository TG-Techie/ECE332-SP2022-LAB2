// #include "address_map_arm.h"

#define KEY_BASE              0xFF200050
#define VIDEO_IN_BASE         0xFF203060
#define FPGA_ONCHIP_BASE      0xC8000000

#include "stdio.h"
// #include "stdlib.h"
// #include "string.h"
#include "stdbool.h"

#include "image.h"
#include "callback.h"

static volatile int * KEY_ptr			= (int *) KEY_BASE; // key peripheral 
static volatile int * Video_In_DMA_ptr	= (int *) VIDEO_IN_BASE; // the peripheral
static volatile short * Video_Mem_ptr	= (short *) FPGA_ONCHIP_BASE; // vga output memory location


void enable_dma() {
	*(Video_In_DMA_ptr + 3) = 0x4;
}

void disable_dma() {
	*(Video_In_DMA_ptr + 3) = 0x0;
}

bool dma_enabled(void) {
	return (*(Video_In_DMA_ptr + 3) & 0x4) == 0x4;
}

bool dma_disabled(void) {
	return !dma_enabled();
}

void toggle_dma() {
	if (dma_enabled()) {
		disable_dma(NULL);
	} else {
		enable_dma(NULL);
	}
}

/* This program demonstrates the use of the D5M camera with the DE1-SoC Board
 * It performs the following: 
 * 	1. Capture one frame of video when any key is pressed.
 * 	2. Display the captured frame when any key is pressed.		  
*/
/* Note: Set the switches SW1 and SW2 to high and rest of the switches to low for correct exposure timing while compiling and the loading the program in the Altera Monitor program.
*/



void wait_for_keypress(uint8_t mask, callback_t onpress) {
	while(!(*KEY_ptr & mask)) {
		// wait for keypress
	}
	while(*KEY_ptr & mask) {
		// wait for key release
	}
	callback_call(onpress);
	return;
}

void poll_for_keypress(uint8_t mask, callback_t onpress) {
	if(*KEY_ptr & mask) {
		callback_call(onpress);
	}
	return;
}


int main(void)
{	
	enable_dma(NULL);


	while (1) {

		wait_for_keypress(
			0x1,
			(callback_t){ .func = &toggle_dma, .arg = NULL}
		);

		if (!dma_enabled()) {
			// TODO: Copy the video memory to the image buffer for the source arg to this function
			filter_t filters[] = {&bw_filter, &invert_filter, NULL};
			apply_effects(
				(pixel_t*)Video_Mem_ptr,
				(pixel_t*)Video_Mem_ptr, 
				(effects_t){
					.width = 320,
					.height = 240,
					.filters = filters,
					.transforms = NULL,
				}
			);
		}

		// while (!dma_enabled()) {
		// 	poll_for_keypress(0x1, (callback_t){ .func = &toggle_dma, .arg=NULL});

		// }
			



		// enable_dma();				// Enable the video
		// // show the video until a key is pressed to capture a frame
		// for (;;) {
		// 	if ( ! (*KEY_ptr == 0))					// check if any KEY was pressed
		// 	{
		// 		disable_dma();			// Disable the video to capture one frame
		// 		while (*KEY_ptr != 0) {}				// wait for pushbutton KEY release
		// 		break;
		// 	}
		// }

		// // display that frame until a key is pressed again
		// for (;;) {
		// 	if ( ! (*KEY_ptr == 0))					// check if any KEY was pressed
		// 	{
		// 		break;
		// 	}
		// }
	
		// do_memory_shit();

		// printf("reenable dma\n");
		// enable_dma();

	}
}
