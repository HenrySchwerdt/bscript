#ifndef bs_lex_h
#define bs_lex_h

#include "bs.h"

typedef enum {
    // Single-character tokens
    TK_LEFT_PAREN, TK_RIGHT_PAREN,                      // (, ),
    TK_LEFT_BRACE, TK_RIGHT_BRACE,                      // {, },
    TK_LEFT_BRACKET, TK_RIGHT_BRACKET,                  // [, ],
    TK_COMMA, TK_DOT,               // ,, ., ~, !,
    TK_COLON, TK_SEMICOLON, TK_QUESTION_MARK,           // :, ;, ?,
    TK_BIT_NOT, TK_BANG,

    // One or two character tokens
    TK_MINUS,
    TK_MINUS_MINUS,           // -, -=, --,
    TK_PLUS, 
    TK_PLUS_PLUS,               // +, +=, ++,
    TK_SLASH,                          // /, /=,
    TK_STAR,                             // *, *=,
    TK_BIT_AND,               // &, &&, &=,
    TK_BIT_OR,                // |, ||, |=, 
    TK_EQUAL_EQUAL,                 // =, ==, =>,
    TK_GREATER, 
    TK_BIT_SHIFT_RIGHT,
    TK_GREATER_EQUAL,   // >, >>, >=,
    TK_LESS, 
    TK_BIT_SHIFT_LEFT,
    TK_LESS_EQUAL,
    TK_MINUS_EQUAL, 
    TK_PLUS_EQUAL, 
    TK_SLASH_EQUAL,  
    TK_STAR_EQUAL, 
    TK_AND, 
    TK_BIT_AND_EQUAL, 
    TK_OR,
    TK_BIT_OR_EQUAL, 
    TK_EQUAL,      // <, <<, <=,
    TK_ARROW, 

    // Literals
    TK_IDENTIFIER,                                      // [a-z|A-Z|_]+[a-z|A-Z|_|\d]*
    TK_INT_VAL,                                         // [\d]+ | 0x[a-f|A-F|0-9]+ | 0b[0|1]+
    TK_FLOAT_VAL,                                       // [\d]*.[\d]+ | [\d][.[\d]*]?f
    TK_STRING_VAL,                                      // ".*"

    // Type Keywords
    TK_INT64, TK_UINT64,
    TK_INT32, TK_UINT32,                                // int32, u_int32
    TK_INT16, TK_UINT16,                                // int16, u_int16,
    TK_INT8, TK_UINT8,                                  // int8, u_int8
    TK_BOOL,                                            // bool
    TK_STRING,                                          // string
    TK_FLOAT32, TK_FLOAT64,                             // float32, float64

    // Keywords
    TK_CLASS,                                           // class
    TK_STRUCT,                                          // struct
    TK_IF,                                              // if
    TK_ELSE,                                            // else  
    TK_TRUE,                                            // true      
    TK_FALSE,                                           // false
    TK_FOR,                                             // for
    TK_WHILE,                                           // while
    TK_FN,                                              // fn
    TK_NULL,                                            // null
    TK_RETURN,                                          // return
    TK_SUPER,                                           // super
    TK_THIS,                                            // this
    TK_VAR,                                             // var
    TK_VAL,                                             // val
    

    TK_ERROR,
    TK_EOF, 
} bs_token_type;

typedef struct {
    bs_token_type type;
    const char* start;
    int length;
    int line;
} bs_token;


void bs_lex_init(const char* source);
void bs_lex_free();
bs_token next_token(void);

#endif