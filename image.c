#include <stdlib.h>

#include "image.h"

#ifndef IMAGE_BW_BW_THRESHOLD
#define IMAGE_BW_BW_THRESHOLD (uint8_t) 64
#endif

// conveinice defines, undefined at the bottom of the file 
#define u8 uint8_t

// --- stage 1---

pos_t mirror_x_transform(pos_t pix) {
    return pix;
}

pixel_t bw_filter(pixel_t pix) {
    u8 r = pix.r << 3;
    u8 g = pix.g << 2;
    u8 b = pix.b << 3;

    u8 avg = (r/3 + g/3 + b/3);

    if (avg >= IMAGE_BW_BW_THRESHOLD) {
        return white;
    } else {
        return black;
    }
}

pixel_t invert_filter(pixel_t pix) {
    return (pixel_t){
        .r = white.r - pix.r,
        .g = white.g - pix.g,
        .b = white.b - pix.b,
    };
}



void apply_effects(pixel_t* source, pixel_t* dest,  effects_t effects) {

	int y;
	for (y = 0; y < effects.height; y++) {
		int x;
		for (x = 0; x < effects.width; x++) {
            // TODO, de-param for width and height sizes
			pixel_t pix = *(source + (y << 9) + x);
            pos_t pos = (pos_t){.x = x, .y = y};

            // apply each filter if a list of filters is provided
            if (effects.filters != NULL) {
                int idx; // index of the currently active filter, null terminated
                for (idx = 0; effects.filters[idx]; idx++) {
                    pix = effects.filters[idx](pix);
                }
            }

            // //apply each transform if a list of transforms is provided
            // if (effects.transforms != NULL) {
            //     int idx; // index of the currently active transform, null terminated
            //     for (idx = 0; effects.transforms[idx]; idx++) {
            //         pos = effects.transforms[idx](pos, effects.width, effects.height);
            //     }
            // }
			
			// // if the pixel was transformed, out of the image bounds
            // // skip writing that pixel
			// if (pos.x >= effects.width || pos.y >= effects.height) {
            //     continue;
            // }

            *(dest + (pos.y << 9) + pos.x) = pix;
		}
	}
}

// undefine conveinice defines
#undef u8