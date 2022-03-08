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

// interface header include
#include "keys.h"


bool key_pressed() {
    return *KEY_ptr != 0;
}

keys_t wait_for_release() {
    keys_t pressed = pressed = *KEY_ptr;
    // while still pressed, do nothing
    while (*KEY_ptr & pressed) {
    }
    // mask off the key code in case there are other bits set
    return pressed & key_mask;
}

keys_t wait_for_press() {
    while (!key_pressed()) {
    }
    return wait_for_release();
}

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
