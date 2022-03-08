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
#include <stdbool.h>
#include <stdint.h>
// local includes
#include "effects.h"
#include "peripherals.h"

/// @brief an into which maps each bit to a button on the board
typedef uint8_t keys_t;

/// @brief mapping of button key bits to the corresponding effect name
typedef enum {
    bw_key        = 1 << 0,
    bw_invert_key = 1 << 1,
    mirror_x_key  = 1 << 2,
    mirror_y_key  = 1 << 3,
    any_key       = bw_key | bw_invert_key | mirror_x_key | mirror_y_key,
    key_mask      = any_key,
} key_code_t;

/// @brief used to check if any button is pressed
/// @note this only detects button presses, not releases.
///       use `wait_for_release` to detect button release and the pressed keys
/// @return true if the button is pressed, false otherwise
bool key_pressed();

/// @brief waits for a button to be pressed and tells you the pressed key(s)
/// @return the pressed key(s)
keys_t wait_for_release();

/// @brief waits for a button to be pressed and tells you the pressed key(s)
/// @return the pressed key(s)
keys_t wait_for_press();

/// @brief maps a single key press to it's associated effects and transforms
/// @param key the key that was pressed
/// @param filter_p pointer to where the filter should be written
/// @param transform_p pointer to where the transform should be written
/// @return void
void effects_for_key(key_code_t key, filter_t* filter_p, transform_t* transform_p);
