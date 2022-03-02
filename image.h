
#ifndef IMAGE_H
#define IMAGE_H

#include "stdint.h"



/// a type that represent one rgb565 encoded pixel
typedef struct {
    uint16_t b:5;
    uint16_t g:6;
    uint16_t r:5;
} pixel_t;

// x y coordinate of a pixel
typedef struct {
    uint16_t x;
    uint16_t y;
} pos_t;


// -- transform callbacks ---
typedef pixel_t (*filter_t)(pixel_t);
typedef pos_t (*transform_t)(pos_t, uint16_t width, uint16_t height);

typedef struct {
    uint16_t width;
    uint16_t height;
    filter_t* filters;
    transform_t* transforms;
} effects_t;


// --- constants ---

static const pixel_t red = {
    .r = 0x1f, 
    .g = 0x00,
    .b = 0x00,
};

static const pixel_t white = {
    .r = 0x1f, 
    .g = 0x3f,
    .b = 0x1f,
};

static const pixel_t black = {
    .r = 0x00, 
    .g = 0x00,
    .b = 0x00,
};



// --- Stage 1 --

/// applies a set of filters and transforms to an image while writing it
/// out to a destination out to a separate buffer. The source and destination
/// cannot overlap (or be the same buffer).
/// @param source the source image
/// @param dest the destination image
/// @param filters the filters to apply, each can be null or a null temrinated list of filter functions (must conformt to the filter_t type)
/// @param transforms the transforms to apply, each can be null or a null temrinated list of transform functions (must conformt to the transform_t type)
void apply_effects(pixel_t* source,   pixel_t* dest,  effects_t effects);

// useses a global average filter between the three pixels
// subjexct to change
// @protocol: filter_t
// @param threshold: the threshold for the average filter
pixel_t bw_filter(pixel_t pix);

// takes the inverse of the pixel, 5 red becomes 64-5 red
// @protocol: filter_t
pixel_t invert_filter(pixel_t);

// --- stage 2 ---

// TODO: doc comment for this function
pos_t mirror_x_transform(pos_t pos, uint16_t width, uint16_t height);

// TODO: doc comment for this function
pos_t mirror_y_transform(pos_t pos, uint16_t width, uint16_t height);

#endif