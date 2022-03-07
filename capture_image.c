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
#include <stdlib.h>
// local includes
#include "peripherals.h"
#include "effects.h"
#include "keys.h"
#include "video.h"

int main(void) {	
	// initialize the image transfer and statful values for the images
	enable_video();
	int photo_count = 0;

	// ----- the nitty gritty -----
	for (;;) {
		// make sure the timestamp is up to date when not paused on an image
		if (video_enabled()) {
			refresh_timestamp();
		}

		if (key_pressed()) {
			// capture the key press
			keys_t key_code = wait_for_release();
			
			// take the photo, save it
			disable_video();
			save_snapshot();
			
			photo_count += 1;
			refresh_photo_count(photo_count);
			
			// create the list of effects to apply based on the key pressed, here we're putting them right into the list
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

			// wait here to re-enable the video capture so the timestamp and photo count don't change
			wait_for_press();
			enable_video();
		}
	}
}
