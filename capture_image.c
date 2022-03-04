#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "image.h"
#include "callback.h"

// - internal - global buffer for image processing
#define BUFFER_SIZE (size_t)(649*480*sizeof(pixel_t))
pixel_t buffer[BUFFER_SIZE] = {0};


/// --- CUSTOMIZABLE ---

// time zone for timestamp function
#define EST (+7)

// filters:
static const filter_t filters[] = {NULL};
// - bw_filter
// - invert_filter
// - MUST BE null terminated

// transforms:
static const transform_t transforms[] = {&mirror_y_transform, NULL};
// - mirror_x_transform
// - mirror_y_transform
// - MUST BE null terminated

/// --- peripherals ---
#define KEY_BASE              0xFF200050
#define VIDEO_IN_BASE         0xFF203060
#define FPGA_ONCHIP_BASE      0xC8000000
#define CHAR_BUF_CTRL_BASE		0xFF203030
#define FPGA_CHAR_START			0xC9000000


static volatile int * KEY_ptr			= (int *) KEY_BASE; // key peripheral 
static volatile int * Video_In_DMA_ptr	= (int *) VIDEO_IN_BASE; // the peripheral
static volatile short * Video_Mem_ptr	= (short *) FPGA_ONCHIP_BASE; // vga output memory location

static volatile int * video_char_ctrl0  = (int *) CHAR_BUF_CTRL_BASE; // the FPGA
static volatile char * video_char_overlay0 = (char*) FPGA_CHAR_START; // the FPGA

/// --- dma control and related callbacks ---

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

/// --- screen formatting ---

void set_photo_count(int count) {
	char message[] = "photos: %0d";
	snprintf(message, strlen(message), message, count);

	memcpy((void *)(video_char_overlay0 + 1), message, strlen(message));
}

// TODO: docs go here
void refresh_timestamp(){
	time_t rawtime;
	
	// get the current time from the system
	time(&rawtime);

	// localize the time to EST anf format it for strftime
	struct tm * info;
	info = localtime(&rawtime);
	info->tm_hour = (info->tm_hour+EST)%24; 

	// make the formatted "string"
	char message[30];
	strftime(message, 30, "time: %x - %I:%M%p", info);

	// write it to the character overlay peripheral 
	memcpy((void *)(video_char_overlay0 + 14), message, strlen(message));
}

/// --- event based control flow helpers ---

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

/// --- main (duh?) ---

int main(void)
{	

	enable_dma(NULL);

	int photo_count = 0;
	set_photo_count(0);

	refresh_timestamp();

	while (1) {

		wait_for_keypress(
			0x1,
			(callback_t){ .func = &toggle_dma, .arg = NULL}
		);

		if (!dma_enabled()) {

			photo_count += 1;
			set_photo_count(photo_count);
			
			// copy yhe memroy out so it can be tralsted on return
			memcpy(buffer, (pixel_t*)Video_Mem_ptr, BUFFER_SIZE);

			// apply the filters and transforms while writing the image back to the video memory			
			apply_effects(
				buffer,
				(pixel_t*)Video_Mem_ptr, 
				(effects_t){
					.width = 320,
					.height = 240,
					.filters = filters,
					.transforms = transforms,

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
