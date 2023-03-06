#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bs_lex.h"

typedef struct
{
    const char *file_start;
    const char *start;
    const char *current;
    int line;
} bs_lexer;

bs_lexer lexer;

//--------------------------------------
// Private Functions

static bool is_end()
{
    return *lexer.current == '\0';
}

static void next()
{
    lexer.current++;
}

static char advance()
{
    lexer.current++;
    return *(lexer.current - 1);
}

static char current()
{
    return *lexer.current;
}

static char peek()
{
    if (*lexer.current != '\0')
    {
        return *(lexer.current + 1);
    }
    return '\0';
}

static bool match(char expected)
{
    if (is_end())
        return false;
    if (*lexer.current != expected)
        return false;
    lexer.current++;
    return true;
}

static void skip_whitespaces_and_comments()
{
    for (;;)
    {
        switch (current())
        {
        case ' ':
        case '\r':
        case '\t':
            next();
            break;
        case '\n':
            lexer.line++;
            next();
            break;
        case '/':
            if (peek() == '/')
            {
                while (peek() != '\n' && !is_end())
                    next();
                next();
                break;
            }
            else if (peek() == '*')
            {
                while (!(current() == '*' && peek() == '/') && !is_end())
                {
                    if (current() == '\n')
                        lexer.line++;
                    next();
                }
                next(); // current = '/'
                next(); // current = Next Char
                break;
            }
            else
            {
                return;
            }
        default:
            return;
        }
    }
}

static bool is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

static bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static bs_token create_token(bs_token_type type)
{
    bs_token token;
    token.type = type;
    token.start = lexer.start;
    token.length = (int)(lexer.current - lexer.start);
    token.line = lexer.line;
    return token;
}

static bs_token_type check_keyword(int start, int length, const char *rest, bs_token_type type)
{
    if (lexer.current - lexer.start == start + length && memcmp(lexer.start + start, rest, length) == 0)
    {
        return type;
    }

    return TK_IDENTIFIER;
}

static bs_token_type check_uint_type()
{
    if (memcmp(lexer.start, "u_int", 5) == 0)
    {
        if (lexer.current - lexer.start == 6 && lexer.start[5] == '8')
            return TK_UINT8;
        if (lexer.current - lexer.start == 7 && lexer.start[5] == '1' && lexer.start[6] == '6')
            return TK_UINT16;
        if (lexer.current - lexer.start == 7 && lexer.start[5] == '3' && lexer.start[6] == '2')
            return TK_UINT32;
        if (lexer.current - lexer.start == 7 && lexer.start[5] == '6' && lexer.start[6] == '4')
            return TK_UINT64;
    }
    return TK_IDENTIFIER;
}

static bs_token_type check_int_type()
{
    // int8 int16 int32
    if (memcmp(lexer.start, "int", 3) == 0)
    {
        if (lexer.current - lexer.start == 4 && lexer.start[3] == '8')
            return TK_INT8;
        if (lexer.current - lexer.start == 5 && lexer.start[3] == '1' && lexer.start[4] == '6')
            return TK_INT16;
        if (lexer.current - lexer.start == 5 && lexer.start[3] == '3' && lexer.start[4] == '2')
            return TK_INT32;
        if (lexer.current - lexer.start == 5 && lexer.start[3] == '6' && lexer.start[4] == '4')
            return TK_INT64;
    }
    return TK_IDENTIFIER;
}

static bs_token_type check_float_type()
{
    if (memcmp(lexer.start, "float", 4) == 0)
    {
        if (lexer.current - lexer.start == 6 && lexer.start[4] == '3' && lexer.start[5] == '2')
            return TK_FLOAT32;
        if (lexer.current - lexer.start == 6 && lexer.start[4] == '6' && lexer.start[5] == '4')
            return TK_FLOAT64;
    }
    return TK_IDENTIFIER;
}

static bs_token_type identifierType()
{
    switch (lexer.start[0])
    {
    case 'b':
        return check_keyword(1, 3, "ool", TK_BOOL);
    case 'c':
        return check_keyword(1, 4, "lass", TK_CLASS);
    case 'e':
        return check_keyword(1, 3, "lse", TK_ELSE);
    case 'i':
    {
        if (lexer.current - lexer.start > 1)
        {
            switch (lexer.start[1])
            {
            case 'f':
                return lexer.current - lexer.start == 2 ? TK_IF : TK_IDENTIFIER;
            case 'n':
                return check_int_type();
            }
        }
        break;
    }
    case 'n':
        return check_keyword(1, 3, "ull", TK_NULL);
    case 'r':
        return check_keyword(1, 5, "eturn", TK_RETURN);
    case 's':
    {
        if (lexer.current - lexer.start > 1)
        {
            switch (lexer.start[1])
            {
            case 'u':
                return check_keyword(2, 3, "per", TK_SUPER);
            case 't':
                if (lexer.current - lexer.start > 2)
                {
                    switch (lexer.start[2])
                    {
                    case 'r':
                        if (lexer.current - lexer.start > 3)
                        {
                            switch (lexer.start[3])
                            {
                            case 'i':
                                return check_keyword(4, 2, "ng", TK_STRING);
                            case 'u':
                                return check_keyword(4, 2, "ct", TK_STRUCT);
                            }
                        }
                    }
                }
            }
        }
        break;
    }
    case 't':
        if (lexer.current - lexer.start > 1)
        {
            switch (lexer.start[1])
            {
            case 'h':
                return check_keyword(2, 2, "is", TK_THIS);
            case 'r':
                return check_keyword(2, 2, "ue", TK_TRUE);
            }
        }
        break;
    case 'f':
        if (lexer.current - lexer.start > 1)
        {
            switch (lexer.start[1])
            {
            case 'l':
                return check_float_type();
            case 'a':
                return check_keyword(2, 3, "lse", TK_FALSE);
            case 'o':
                return check_keyword(2, 1, "r", TK_FOR);
            case 'n':
                return lexer.current - lexer.start == 2 ? TK_FN : TK_IDENTIFIER;
            }
        }
        break;
    case 'u':
        return check_uint_type();
    case 'v':
        if (lexer.current - lexer.start > 1)
        {
            switch (lexer.start[1])
            {
            case 'a':
                if (lexer.current - lexer.start == 3)
                {
                    switch (lexer.start[2])
                    {
                    case 'r':
                        return TK_VAR;
                    case 'l':
                        return TK_VAL;
                    }
                }
                return TK_IDENTIFIER;
            }
        }
        break;
    case 'w':
        return check_keyword(1, 4, "hile", TK_WHILE);
    }

    return TK_IDENTIFIER;
}

static bs_token identifier()
{
    while (is_alpha(current()) || is_digit(current()))
        advance();

    return create_token(identifierType());
}

static bs_token floatDot()
{
    while (is_digit(peek()) && !is_end())
    {
        advance();
    }
    if (peek() == 'f')
    {
        advance();
    }
    advance();
    return create_token(TK_FLOAT_VAL);
}

// [0-9][.[0-9]+]?f?
static bs_token number()
{
    while (is_digit(current()) && !is_end())
    {
        advance();
    }
    if (current() == '.')
    {
        advance();
        while (is_digit(current()) && !is_end())
        {
            advance();
        }
        if (current() == 'f')
        {
            advance();
        }
        return create_token(TK_FLOAT_VAL);
    }
    if (current() == 'f')
    {
        advance();
        return create_token(TK_FLOAT_VAL);
    }
    return create_token(TK_INT_VAL);
}

static bs_token string()
{
    while (peek() != '"' && !is_end())
    {
        if (peek() == '\n')
            lexer.line++;
        advance();
    }
    // if (is_end()) // TODO return errorToken;
    //     printf("unterminated String\n");
    advance(); // current = "
    advance(); // current = new Char
    return create_token(TK_STRING_VAL);
}

//--------------------------------------
// Public Functions
void bs_lex_init(const char *source)
{
    lexer.file_start = source;
    lexer.start = source;
    lexer.current = source;
    lexer.line = 1;
}

void bs_lex_free()
{
    lexer.file_start = NULL;
    lexer.start = NULL;
    lexer.current = NULL;
    lexer.line = 1;
}

bs_token next_token()
{
    skip_whitespaces_and_comments();
    lexer.start = lexer.current;
    if (is_end())
        return create_token(TK_EOF);
    char c = advance();
    if (is_alpha(c))
        return identifier();
    if (is_digit(c))
        return number();
    switch (c)
    {
    // single character tokens
    case '(':
        return create_token(TK_LEFT_PAREN);
    case ')':
        return create_token(TK_RIGHT_PAREN);
    case '{':
        return create_token(TK_LEFT_BRACE);
    case '}':
        return create_token(TK_RIGHT_BRACE);
    case '[':
        return create_token(TK_LEFT_BRACKET);
    case ']':
        return create_token(TK_RIGHT_BRACKET);
    case ',':
        return create_token(TK_COMMA);
    case '.':
        return is_digit(current()) ? floatDot() : create_token(TK_DOT);
    case '~':
        return create_token(TK_BIT_NOT);
    case ':':
        return create_token(TK_COLON);
    case ';':
        return create_token(TK_SEMICOLON);
    case '?':
        return create_token(TK_QUESTION_MARK);
    case '!':
        return create_token(TK_BANG);
    // single or double character tokens
    case '-':
    {
        switch (current())
        {
        case '-':
            next();
            return create_token(TK_MINUS_MINUS);
        case '=':
            next();
            return create_token(TK_MINUS_EQUAL);
        default:
            return create_token(TK_MINUS);
        }
    }
    case '+':
    {
        switch (current())
        {
        case '+':
            next();
            return create_token(TK_PLUS_PLUS);
        case '=':
            next();
            return create_token(TK_PLUS_EQUAL);
        default:
            return create_token(TK_PLUS);
        }
    }
    case '/':
        return create_token(match('=') ? TK_SLASH_EQUAL : TK_SLASH);
    case '*':
        return create_token(match('=') ? TK_STAR_EQUAL : TK_STAR);
    case '&':
    {
        switch (current())
        {
        case '=':
            next();
            return create_token(TK_BIT_AND_EQUAL);
        case '&':
            next();
            return create_token(TK_AND);
        default:
            return create_token(TK_BIT_AND);
        }
    }
    case '|':
    {
        switch (current())
        {
        case '=':
            next();
            return create_token(TK_BIT_OR_EQUAL);
        case '|':
            next();
            return create_token(TK_OR);
        default:
            return create_token(TK_BIT_OR);
        }
    }
    case '=':
    {
        switch (current())
        {
        case '=':
            next();
            return create_token(TK_EQUAL_EQUAL);
        case '>':
            next();
            return create_token(TK_ARROW);
        default:
            return create_token(TK_EQUAL);
        }
    }
    case '>':
    {
        switch (current())
        {
        case '=':
            next();
            return create_token(TK_GREATER_EQUAL);
        case '>':
            next();
            return create_token(TK_BIT_SHIFT_RIGHT);
        default:
            return create_token(TK_GREATER);
        }
    }
    case '<':
    {
        switch (current())
        {
        case '=':
            next();
            return create_token(TK_LESS_EQUAL);
        case '<':
            next();
            return create_token(TK_BIT_SHIFT_LEFT);
        default:
            return create_token(TK_LESS);
        }
    }
    case '"':
        return string();
    }
    return create_token(TK_ERROR);
}