#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "image.h"

/// --- defs / etc ---
// time zone for timestamp function
#define EST (+7)

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

void enable_video() {
	*(Video_In_DMA_ptr + 3) = 0x4;
}

void disable_video() {
	*(Video_In_DMA_ptr + 3) = 0x0;
}

bool video_enabled(void) {
	return (*(Video_In_DMA_ptr + 3) & 0x4) == 0x4;
}

void toggle_video() {
	if (video_enabled()) {
		disable_video(NULL);
	} else {
		enable_video(NULL);
	}
}

/// --- image saving and utils ---
// - internal - global buffer for image processing
#define SNAPSHOT_SIZE_BYTES (size_t)(649*480*sizeof(pixel_t))
pixel_t snapshot_buffer[SNAPSHOT_SIZE_BYTES] = {0};
#define video_memory (pixel_t*)Video_Mem_ptr

// TODO: docs
void save_snapshot() {
	// copy the memroy out so it can be tralsted on return
	memcpy(snapshot_buffer, video_memory, SNAPSHOT_SIZE_BYTES);
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

typedef uint8_t keys_t;

typedef enum key_code {
	bw_key 			= 1<<0,
	bw_invert_key	= 1<<1,
	mirror_x_key 	= 1<<2,
	mirror_y_key 	= 1<<3,
	any_key 		= bw_key | bw_invert_key | mirror_x_key | mirror_y_key,
	key_mask 		= any_key,
} key_code_t;

bool key_pressed() {
	return *KEY_ptr != 0;
}

key_code_t wait_for_release() {
	// while(!(*KEY_ptr & mask)) {
	// 	// wait for keypress
	// }
	keys_t pressed = pressed = *KEY_ptr;
	while (*KEY_ptr & pressed) {
		// wait for key release
	}
	return pressed & key_mask;
}

key_code_t wait_for_press() {
	while (!key_pressed()) {}
	return wait_for_release();
}

// TODO: docs go here, note taht the filter and transfrom are return destination pointers 
void effects_for_key(key_code_t key, filter_t* filter_p, transform_t* transform_p) {
	key = key & key_mask;

	switch (key) {
	case bw_key:
		*filter_p = &bw_filter;
		break;
	case bw_invert_key:
		*filter_p = &bw_invert_filter;
		break;
	case mirror_x_key:
		*transform_p = &mirror_x_transform;
		break;
	case mirror_y_key:
		*transform_p = &mirror_y_transform;
		break;
	default:
		break;
	}
}

/// --- main (duh?) ---

int main(void) {	
	// initialize the image transfer 
	enable_video();

	// setup the stateful variables
	int photo_count = 0;

	// main loop
	for (;;) {
		if (video_enabled()) {
			refresh_timestamp();
		}


		if (key_pressed()) {
			// capture the key press
			key_code_t key_code = wait_for_release();
			
			// take the photo, save it
			disable_video();
			save_snapshot();
			// update the photo count accordingly
			photo_count += 1;
			set_photo_count(photo_count);
			
			// create the list of effects to apply
			filter_t filters[] = {NULL, NULL};
			transform_t transforms[] = {NULL, NULL};
			effects_for_key(key_code, &filters[0], &transforms[0]);

			// apply the effects and display it back into the video memory
			apply_effects(
	 			snapshot_buffer,
				video_memory, 
				(effects_t) {
					.width = 320,
					.height = 240,
					.filters = filters,
					.transforms = transforms,
				}
			);

			// wait here to re-enable the video capture so 
			// the timestamp and photo count don't change
			wait_for_press();
			enable_video();
		}
	}
}
