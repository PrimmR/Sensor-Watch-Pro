/*
 * MIT License
 *
 * Copyright (c) <#year#> <#author_name#>
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

#include <stdlib.h>
#include <string.h>
#include "pride_face.h"

#include "watch.h"
#include "watch_slcd.h"
#include "watch_common_display.h"

void _cycle_leds(pride_state_t *state) {
    if (state->col.red == 0xF && state->col.green == 0xF)
        state->col.red--;
    else if (state->col.green == 0xF && state->col.blue == 0xF)
        state->col.green--;
    else if (state->col.blue == 0xF && state->col.red == 0xF)
        state->col.blue--;
    else if (state->col.red == 0xF) {
        if (state->col.blue != 0x0)
            state->col.blue--;
        else
            state->col.green++;
    }
    else if (state->col.green == 0xF) {
        if (state->col.red != 0x0)
            state->col.red--;
        else
            state->col.blue++;
    }
    else if (state->col.blue == 0xF) {
        if (state->col.green != 0x0)
            state->col.green--;
        else
            state->col.red++;
    }

    movement_force_led_on(state->col.red, state->col.green, state->col.blue);
}

void pride_face_setup(uint8_t watch_face_index, void ** context_ptr) {
    (void) watch_face_index;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(pride_state_t));
        memset(*context_ptr, 0, sizeof(pride_state_t));
        // Do any one-time tasks in here; the inside of this conditional happens only at boot.
    }
    // Do any pin or peripheral setup here; this will be called whenever the watch wakes from deep sleep.
}

void pride_face_activate(void *context) {
    pride_state_t *state = (pride_state_t *)context;

    state->col = (movement_color_t) {
        .red = 0xF, .green = 0, .blue = 0
    };
    state->active = false;

    movement_request_tick_frequency(16);
}

bool pride_face_loop(movement_event_t event, void *context) {
    pride_state_t *state = (pride_state_t *)context;

    switch (event.event_type) {
        case EVENT_ACTIVATE:
            // Show your initial UI here.
            #if defined(WATCH_BLUE_TCC_CHANNEL) && defined(WATCH_GREEN_TCC_CHANNEL)
                watch_display_text(WATCH_POSITION_BOTTOM, " Pride");
            #else
                watch_display_text(WATCH_POSITION_TOP_LEFT, "No");
                watch_display_text_with_fallback(WATCH_POSITION_BOTTOM, " LED", " led");
            #endif
            break;
        case EVENT_TICK:
        {
            #if defined(WATCH_BLUE_TCC_CHANNEL) && defined(WATCH_GREEN_TCC_CHANNEL)
                watch_display_text(WATCH_POSITION_BOTTOM, " Pride");
                if (state->active) {
                    _cycle_leds(state);
                    watch_set_indicator(WATCH_INDICATOR_SIGNAL);
                } else {
                    movement_force_led_off();
                    watch_clear_indicator(WATCH_INDICATOR_SIGNAL);
                }
            #else
                watch_display_text(WATCH_POSITION_TOP_LEFT, "No");
                watch_display_text(WATCH_POSITION_BOTTOM, " LED");
            #endif
            break;
        }
        case EVENT_LIGHT_BUTTON_UP:
        case EVENT_LIGHT_LONG_UP:
        case EVENT_LIGHT_LONG_PRESS:
            break;
        case EVENT_LIGHT_BUTTON_DOWN:
            // Prevents regular light activating
            {
                state->active = !state->active;
                movement_force_led_off();
            }
            break;
        case EVENT_TIMEOUT:
            movement_move_to_face(0);
            break;
        default:
            return movement_default_loop_handler(event);
    }
    return true;
}

void pride_face_resign(void *context) {
    (void) context;

    // handle any cleanup before your watch face goes off-screen.
    movement_force_led_off();
    watch_clear_indicator(WATCH_INDICATOR_SIGNAL);
}

