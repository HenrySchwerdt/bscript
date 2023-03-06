#ifndef bs_error_h
#define bs_error_h
#include <stdio.h>

#include "bs.h"
#include "bs_lex.h"

// File Errors
#define COULD_NOT_OPEN_FILE "Could not open file"
#define NOT_ENOUGH_MEMORY_FILE "Not enough memory to read"
#define COULD_NOT_READ_FILE "Could not read file"


#define RUN_TIME_ERROR()
#define COMPILE_ERROR(line_nr, name, message) error_at(line_nr, name, message)
#define COMMAND_LINE_USAGE_ERROR(command) command_line_error(command)
#define FILE_ERROR(message, filename) file_error(message, filename)
#define USAGE_ERROR() usage_error()

void command_line_error(const char * command);
void usage_error(void);
void file_error(const char * message, const char * filename);
void error_at(int line_nr, bs_token * name, const char * message);

#endif