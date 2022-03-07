// MIT License

// Copyright (C) 2022 Jonah "Jay" Yolles-Murphy, Khadija Ben-Neticha

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in 
// the Software without restriction, including without limitation the rights to 
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
// of the Software, and to permit persons to whom the Software is furnished to 
// do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// system includes
#include <string.h>
#include <time.h>
// interface header include
#include "video.h"
// local includes 
#include "peripherals.h"

/// ----- defs / etc -----
// time zone for timestamp function
#define EST (+7)


/// ----- dma control to enable and control the camera input -----
void enable_video() {
	// TODO: make a bitfeild based type to make this clearer
	// set the enable bit high
	*(Video_In_DMA_ptr + 3) = 0x4;
}

void disable_video() {
	// set the enable bit low
	*(Video_In_DMA_ptr + 3) = 0x0;
}

bool video_enabled(void) {
	return (*(Video_In_DMA_ptr + 3) & 0x4) == 0x4;
}


/// ----- image saving and utils -----
pixel_t snapshot_buffer[SNAPSHOT_SIZE_BYTES] = {0};
volatile pixel_t* const video_memory = (pixel_t * const)FPGA_ONCHIP_VIDEO_BASE;

// TODO: docs
void save_snapshot() {
	// copy the memory out so it can be translated on return
	memcpy(snapshot_buffer, video_memory, SNAPSHOT_SIZE_BYTES);
}

/// ----- screen formatting -----

//adds photo counter to top left of image
void refresh_photo_count(int count) {
	char message[] = "photos: %0d";
	snprintf(message, strlen(message), message, count);

	memcpy((void *)(video_char_overlay0 + 1), message, strlen(message));
}

// TODO: docs go here
void refresh_timestamp(){
	time_t rawtime;
	
	// get the current time from the system
	time(&rawtime);

	// localize the time to EST and format it for strftime
	struct tm * info;
	info = localtime(&rawtime);
	info->tm_hour = (info->tm_hour+EST)%24; 

	// make the formatted "string"
	char message[30];
	strftime(message, 30, "time: %x - %I:%M%p", info);

	// write it to the character overlay peripheral 
	memcpy((void *)(video_char_overlay0 + 14), message, strlen(message));
}
