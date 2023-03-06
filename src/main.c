#include <stdio.h>
#include <stdlib.h>
#include "bs.h"
#include "bs_parser.h"
#include "bs_ast.h"
#include "bs_error.h"

static char *read_file(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL)
    {
        FILE_ERROR(COULD_NOT_OPEN_FILE, path);
        exit(74);
    }
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);
    char *buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL)
    {
        FILE_ERROR(NOT_ENOUGH_MEMORY_FILE, path);
        exit(74);
    }
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize)
    {
        FILE_ERROR(COULD_NOT_READ_FILE, path);
        exit(74);
    }

    buffer[bytesRead] = '\0';
    fclose(file);
    return buffer;
}

static uint8_t parse_flags(int argc, const char *argv[])
{
    uint8_t flags = 0;
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 'c':
                flags |= COMPILE;
                break;
            case 'a':
                flags |= TO_AST;
                break;
            case 'o':
                flags |= TO_OUTPUT;
                break;
            case 'h':
                flags |= HELP;
                break;
            default:
                USAGE_ERROR();
                exit(64);
            }
        }
    }
    return flags;
}
static void print_help()
{
    printf("Usage:\n");
    printf("bs [-options] [path] [params]\n\n");
    printf("Options:\n");
    printf(" -c : option excepting argument.\n");
    printf(" -a : option without arguments.\n");
    printf(" -o : option without arguments.\n");
    printf(" -h : print out command line options.\n\n");
}

int main(int argc, const char *argv[])
{
    if (argc == 1)
    {
        // repl();
    }
    else if (argc >= 2)
    {
        uint8_t flags = parse_flags(argc, argv);
        if ((flags & HELP) > 0)
        {
            print_help();
            return 0;
        }
        char *source = read_file(argv[argc - 1]);
        AST *ast = parse(source);
        if ((flags & TO_AST) > 0)
        {
            if ((flags & TO_OUTPUT) > 0)
            {
                FILE *f = fopen("ast.json", "w");
                ast_print(ast, f);
                fclose(f);
            }
            else
            {
                ast_print(ast, stdout);
            }
        }
        ast_free(ast);
    }
    return 0;
}
