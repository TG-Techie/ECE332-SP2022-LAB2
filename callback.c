#include <stdlib.h>
#include <stdbool.h>

#include "callback.h"

void callback_call(callback_t cb) {
    if (cb.func != NULL)
        cb.func(cb.arg);
}


void toggle_callback_fn(bool* value) {
	*value = !*value;
}


callback_t toggle_callback(bool* value_p) {
    return (callback_t){.func=&toggle_callback_fn, .arg=value_p};
}