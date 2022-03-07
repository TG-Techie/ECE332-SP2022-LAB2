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

#pragma once

// system includes
#include <stdint.h>
#include <stdbool.h>

// ----- shared pixel respresentation -----

/// @brief A struct representing a rgb565 endcoded pixel. 
/// 
/// This uses bitfeild to seaprate the red, green, and blue channels
/// from each other. Note the channels do not all chare the same max value; 
/// use the `white` constant to get the max value for each channel, eg `mypix.r / white.r`.
typedef struct {
    uint16_t b:5;
    uint16_t g:6;
    uint16_t r:5;
} pixel_t;

// pre-defines pixel values for convenience
static const pixel_t white  = {.r = 0x1f, .g = 0x3f, .b = 0x1f};
static const pixel_t black  = {.r = 0x00, .g = 0x00, .b = 0x00};
static const pixel_t red    = {.r = 0x1f, .g = 0x00, .b = 0x00};

// ----- utility functions to enable/disable video capture (uses dma) -----
/// @section video-utils
///
/// the hardware as provided by the board and quartus file uses a dma channel
/// to stream data from the attached camera into the output buffer of the vga peripheral.
/// Thus, to take a snapshot you to enable/disable the dma channel, this section provides
/// utilities to do so.

/// @brief Starts sreaming video formt eh camera the video capture system. 
/// @return void
void enable_video();

/// @brief Stops streaming video from the camera the video capture system. 
/// @return void
void disable_video();

/// @brief Gets the current state of the video capture system. 
/// @return true if the video capture system is enabled, false otherwise
bool video_enabled(void);

// ----- image saving and utils -----
/// @section snapshot-capture
///
/// This section provides functions to save a snapshot of the current frame
/// and save it to a global buffer for processing/effects later.

/// @brief use for specifycng the size of the global buffer when copying memory 
#define SNAPSHOT_SIZE_BYTES (size_t)(649*480*sizeof(pixel_t))

/// @brief the video buffer used by the vga peripheral
volatile pixel_t* const video_memory;

/// @brief the global snapshot buffer, any snapshot taken will be copied here
volatile pixel_t snapshot_buffer[SNAPSHOT_SIZE_BYTES];

/// @brief saves a snapshot of the current frame to the snapshot buffer
/// @note be sure to pause the video capture system before calling this
/// @return void
void save_snapshot();

// ----- overlay functions -----

/// @brief writes a new photo count to the vga overlay buffer
/// @param count the new photo count to write
/// @return void
void refresh_photo_count(int count);

/// @brief write the current time to the vga overlay buffer, in ESTS format
/// @return void
void refresh_timestamp();
