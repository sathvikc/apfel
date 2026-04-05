#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

static inline FILE *apfel_get_rl_outstream(void) {
    return rl_outstream;
}

static inline void apfel_set_rl_outstream(FILE *stream) {
    rl_outstream = stream;
}

static inline FILE *apfel_get_rl_instream(void) {
    return rl_instream;
}

static inline void apfel_set_rl_instream(FILE *stream) {
    rl_instream = stream;
}
