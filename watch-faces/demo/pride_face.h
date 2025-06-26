/*
 * MIT License
 *
 * Copyright (c) 2025 PrimmR
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "movement.h"

/*
 * PRIDE FACE
 *
 * Press the light button to make the LED cycle through a rainbow gradient
 * Only works with boards that have all 3 colours of LED
 *
 */

typedef struct {
    // Anything you need to keep track of, put it here!
    movement_color_t col;
    bool active;
} pride_state_t;

void pride_face_setup(uint8_t watch_face_index, void ** context_ptr);
void pride_face_activate(void *context);
bool pride_face_loop(movement_event_t event, void *context);
void pride_face_resign(void *context);

#define pride_face ((const watch_face_t){ \
    pride_face_setup, \
    pride_face_activate, \
    pride_face_loop, \
    pride_face_resign, \
    NULL, \
})
