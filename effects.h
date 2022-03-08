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
// local includes
#include "video.h"

// x y coordinate of a pixel
typedef struct {
    uint16_t x;
    uint16_t y;
} pos_t;


// ----- transform callbacks -----

/// @brief a callback that filters a pixel's color
typedef pixel_t (*filter_t)(pixel_t);
/// @brief a callback that transforms a pixel's location
typedef pos_t (*transform_t)(pos_t, uint16_t width, uint16_t height);

/// @brief data strucutre that describes a set of effects to apply to an image
/// @note the filters, and transforms arrays must be null terminated
typedef struct {
    uint16_t width;
    uint16_t height;
    filter_t* filters;
    transform_t* transforms;
} effects_t;


/// @brief applies a set of filters and transforms to an image while writing it
///
/// @note the source destination and effects must be the same size
/// @note the order of the effects is important, filters are applied before transforms.
///       for each array it applies each effect in order until it reaches a null terminator
///
/// @param source the source image
/// @param dest the destination image
/// @param filters the filters to apply, each can be null or a null terminated list of filter functions (must conform to the filter_t type)
/// @param transforms the transforms to apply, each can be null or a null terminated list of transform functions (must conform to the transform_t type)
/// @return none
void apply_effects(const pixel_t* source,   pixel_t* dest,  effects_t effects);

/// @brief returns a black or white pixel of the given color
/// 
/// @note this currently uses a "global average" threshold, but this is subject to change
/// @note the threshold for this can be overriden by defining the `IMAGE_BW_BW_THRESHOLD` macro,
///       The macro should be a value between 0 and 255, defaults to 64 if not defined   
/// 
/// @param pix the pixel to filter
/// @return the filtered pixel
pixel_t bw_filter(pixel_t pix);

/// @brief takes the inverse of the pixel, 5 red becomes 64-5 red and so on
/// 
/// @param pix the pixel to filter
/// @return the filtered pixel
pixel_t invert_filter(pixel_t);

/// @brief in a black and white filter (inverts the black and white)
/// 
/// @param pix the pixel to filter
/// @return the filtered pixel
pixel_t bw_invert_filter(pixel_t);

/// @brief returns the corresponding pixel position when horizontally mirrored accross the center of the image
///
/// @param pos the position to mirror
/// @param width the width of the image
/// @param height the height of the image
/// @return the mirrored position
pos_t mirror_x_transform(pos_t pos, uint16_t width, uint16_t height);

/// @brief returns the corresponding pixel position when vertically mirrored accross the center of the image
///
/// @param pos the position to mirror
/// @param width the width of the image
/// @param height the height of the image
/// @return the mirrored position
pos_t mirror_y_transform(pos_t pos, uint16_t width, uint16_t height);
