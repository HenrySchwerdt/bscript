#ifndef bs_h
#define bs_h

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BS_VERSION_MAJOR "1"
#define BS_VERSION_MINOR "0"
#define BS_VERSION_RELEASE "0"

#define BS_VERSION "Bs " BS_VERSION_MAJOR "." BS_VERSION_MINOR "." BS_VERSION_RELEASE

#define BS_AUTHORS "H. Schwerdtner"

#define BS_SIGNATURE "\x1brbs"

// ARGUMENTS
#define COMPILE 0b00000001
#define TO_AST 0b00000010
#define TO_OUTPUT 0b00000100
#define HELP 0b00001000

#endif