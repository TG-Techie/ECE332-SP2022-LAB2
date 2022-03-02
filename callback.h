
#ifndef CALLBACK_H
#define CALLBACK_H

typedef struct {
    void (*func)(void *);
    void *arg;
} callback_t;

callback_t toggle_callback(bool* value_p);

static const callback_t do_nothing = {
    .func = NULL,
    .arg = NULL,
};

void callback_call(callback_t cb);



#endif