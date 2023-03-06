#include "bs_error.h"

// Regular text
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

// Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

// Reset 
#define RESET "\e[0m"

#define BS  "bs:"
#define USAGE "usage:"
#define BAD_OPTION_ERROR "bad option:"

static void red() {
    fprintf(stderr, "%s", RED);
}

static void bred() {
    fprintf(stderr, "%s", BRED);
}

static void reset() {
    fprintf(stderr, "%s", RESET);
}

static void bold_red(const char * message) {
    bred(); 
    fprintf(stderr, "%s ", message); 
    reset(); 
}

void usage_error() {
    bold_red(BS);
    red(); 
    fprintf(stderr, "%s ", USAGE);
    red();
    fprintf(stderr, "bs [-options] [path] [params]\n");
    fprintf(stderr, " - Use option '-h' for help.\n");
    reset();
}

void command_line_error(const char * command) {
    bold_red(BS);
    red(); 
    fprintf(stderr, "%s '%s'\n", BAD_OPTION_ERROR, command);
    fprintf(stderr, " - Use option '-h' for help.\n");
    reset();
}

void file_error(const char * message, const char * filename){
    bold_red(BS);
    red();
    fprintf(stderr, "%s '%s'\n", message, filename);
    reset();
}

void error_at(int line_nr, bs_token * name, const char * message) {
    red();
    fprintf(stderr, "[line %d] Error", name->line);
    if (name->type == TK_EOF) {
        fprintf(stderr, " at end");
    } else if (name->type == TK_ERROR) {
    } else {
        fprintf(stderr, " at '%.*s'", name->length, name->start);
    }
    fprintf(stderr, ": %s\n", message);
    reset();
}