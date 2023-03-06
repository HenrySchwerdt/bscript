#include "bs_parser.h"
#include "bs_value.h"

typedef struct
{
    bs_token previous;
    bs_token current;
    bool hadError;
    bool panikMode;
} bs_parser;

bs_parser parser;

static bool is_end()
{
    return parser.current.type == TK_EOF;
}

static bs_token advance()
{
    if (!is_end())
    {
        parser.previous = parser.current;
        parser.current = next_token();
    }
    return parser.current;
}

static bs_token current()
{
    return parser.current;
}

static bs_token previous()
{
    return parser.previous;
}

static bool check(bs_token_type type)
{
    if (is_end())
        return false;
    return current().type == type;
}

static bool match(bs_token_type *tokens, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (check(tokens[i]))
        {
            advance();
            return true;
        }
    }
    return false;
}

static bool match_single(bs_token_type token)
{
    if (check(token))
    {
        advance();
        return true;
    }
    return false;
}

static void consume(bs_token_type type, const char *error_message)
{
    if (match_single(type))
    {
        return;
    }
    bs_token current_tk = current();
    error_at(current_tk.line, &current_tk, error_message);
    parser.hadError = true;
}

/*
IDENTIFIER
*/
static AST *identifier()
{
    if (match_single(TK_IDENTIFIER))
    {
        return AST_NEW(IDENTIFIER, parser.previous.start, parser.previous.length);
    }
}

/*
<primary> ::= NUMBER | STRING | "true" | "false" | "null" | "(" expression ")" ;
*/
static AST *primary()
{
    bs_token token = current();
    if (match_single(TK_INT_VAL))
    {
        int8_t value = (int8_t)atoi(token.start);
        bs_value *test = BS_VALUE(BS_INT32, value);
        return AST_NEW(LITERAL, test);
    }
    return identifier();
}

/*
<pos-unary> ::= <primary> ("--" | "++")*
*/
static AST *pos_unary()
{
    bs_token_type tokens[] = {TK_MINUS_MINUS, TK_PLUS_PLUS};
    AST *token = primary();
    if (match(tokens, 2))
    {
        bs_token_type operator= previous().type;
        return AST_NEW(UNARY_EXPRESSION,
                       token,
                       AS_AST_OPERATOR(operator));
    }
    return token;
}

/*
<unary> ::= ( "-" | "!" | "~" | "++" | "--" ) <unary> | <primary> ;
*/
static AST *unary()
{
    bs_token_type tokens[] = {TK_BANG, TK_MINUS, TK_BIT_NOT, TK_MINUS_MINUS, TK_PLUS_PLUS};
    if (match(tokens, 3))
    {
        bs_token_type operator= previous().type;
        return AST_NEW(UNARY_EXPRESSION,
                       unary(),
                       AS_AST_OPERATOR(operator), );
    }
    return pos_unary();
}
/*
<factor> ::= <unary> (( "*" | "/" | "%" ) <unary>)* ;
*/
static AST *factor()
{
    bs_token_type tokens[] = {TK_STAR, TK_SLASH};
    AST *node = unary();
    if (match(tokens, 2))
    {
        bs_token_type operator= previous().type;
        return AST_NEW(
            BINARY_EXPRESSION,
            node,
            unary(),
            AS_AST_OPERATOR(operator));
    }
    return node;
}

/*
<term> ::= <factor> (("-" | "+") <factor>)* ;
*/
static AST *term()
{
    bs_token_type tokens[] = {TK_MINUS, TK_PLUS};
    AST *node = factor();
    if (match(tokens, 2))
    {
        bs_token_type operator= previous().type;
        return AST_NEW(BINARY_EXPRESSION,
                       node,
                       factor(),
                       AS_AST_OPERATOR(operator));
    }
    return node;
}

/*
<shift> ::= <term> (("<<" | ">>") <term>)* ;
*/
static AST *shift()
{
    bs_token_type tokens[] = {TK_BIT_SHIFT_LEFT, TK_BIT_SHIFT_LEFT};
    AST *node = term();
    if (match(tokens, 2))
    {
        bs_token_type operator= previous().type;
        return AST_NEW(BINARY_EXPRESSION,
                       node,
                       term(),
                       AS_AST_OPERATOR(operator));
    }
    return node;
}

/*
<comparison> ::= <term> ((">" | "<" | "<=" | ">=") <term>)* ;
*/
static AST *comparison()
{
    // current: garbage next: 3
    bs_token_type tokens[] = {TK_GREATER, TK_LESS, TK_GREATER_EQUAL, TK_LESS_EQUAL};
    AST *node = shift();
    if (match(tokens, 4))
    {
        bs_token_type operator= previous().type;
        return AST_NEW(BINARY_EXPRESSION,
                       node,
                       shift(),
                       AS_AST_OPERATOR(operator));
    }
    return node;
}

/*
<equality> ::= <comparison> (("==" | "!=") <comparison>)* ;
*/
static AST *equality()
{
    // current: garbage next: 3
    bs_token_type tokens[] = {TK_EQUAL_EQUAL};
    AST *node = comparison();
    if (match(tokens, 1))
    {
        bs_token_type operator= previous().type;
        return AST_NEW(BINARY_EXPRESSION,
                       node,
                       comparison(),
                       AS_AST_OPERATOR(operator));
    }
    return node;
}

/*
<bitwise-and> ::= <equality> ("&" <equality>)* ;
*/
static AST *bitwise_and()
{
    bs_token_type tokens[] = {TK_BIT_AND};
    AST *node = equality();
    if (match(tokens, 1))
    {
        bs_token_type operator= previous().type;
        return AST_NEW(BINARY_EXPRESSION,
                       node,
                       equality(),
                       AS_AST_OPERATOR(operator));
    }
    return node;
}

/*
<bitwise-inc-or> ::= <bitwise-exc-or> ("|" <bitwise-exc-or>) ;
*/
static AST *bitwise_inc_or()
{
    bs_token_type tokens[] = {TK_BIT_OR};
    AST *node = bitwise_and();
    if (match(tokens, 1))
    {
        bs_token_type operator= previous().type;
        return AST_NEW(BINARY_EXPRESSION,
                       node,
                       bitwise_and(),
                       AS_AST_OPERATOR(operator));
    }
    return node;
}

static AST *expression();

/*
<tenary> ::= <bitwise-inc-or> "?" <expression> ":" <expression> ;
*/
static AST *tenary()
{
    AST *test = bitwise_inc_or();
    if (match_single(TK_QUESTION_MARK))
    {
        AST *consequent = expression();
        consume(TK_COLON, "Expect ':' after consequent.");
        AST *alternate = expression();
        return AST_NEW(CONDITIONAL_EXPRESSION, test, consequent, alternate);
    }
    return test;
}

/*
<expression> ::= <equality> ;
*/
static AST *expression()
{
    return tenary();
}

/*
<var-declaration> ::= ("var" | "val") IDENTIFIER (":" <type-identifer>)* ("=" <expression>)* ";"
*/
static AST *declaration()
{
    ast_variable_kind kind;
    bs_value_type type;
    if (match_single(TK_VAL))
    {
        kind = VAL;
    }
    else if (match_single(TK_VAR))
    {
        kind = VAR;
    }
    else
    {
        return expression();
    }
    consume(TK_IDENTIFIER, "Expect an Identifier after variable declaration.");
    AST *identifier = AST_NEW(IDENTIFIER, parser.previous.start, parser.previous.length);
    if (match_single(TK_COLON))
    {
        bs_token_type tokens[] = {TK_INT64, TK_UINT64, TK_INT32, TK_UINT32,
                                  TK_INT16, TK_UINT16, TK_INT8, TK_UINT8, TK_BOOL, TK_STRING,
                                  TK_FLOAT32, TK_FLOAT64};
        if (match(tokens, 12))
        {
            type = parser.previous.type - 42;
        }
    }
    AST *init = NULL;
    if (match_single(TK_EQUAL))
    {
        init = expression();
    }
    consume(TK_SEMICOLON, "Expect an ';' after variable initialization.");
    return AST_NEW(VARIABLE_DECLARATION, AST_NEW(VARIABLE_DECLARATOR, identifier, init), kind, type);
}

AST *block_stmt()
{

    if (match_single(TK_LEFT_BRACE))
    {

        AST **instructions = NULL;
        int instructionCount = 0;
        while (parser.current.type != TK_RIGHT_BRACE && parser.current.type != TK_EOF)
        {
            AST *instruction = block_stmt();
            instructionCount += 1;
            instructions = (AST **)realloc(instructions, instructionCount * (sizeof(AST *)));
            instructions[instructionCount - 1] = instruction;
        }

        consume(TK_RIGHT_BRACE, "Expect '}' at the end of a block.");
        return AST_NEW(BLOCK_STMT, instructionCount, instructions);
    }
    return declaration();
}

AST *parse(const char *source)
{
    bs_lex_init(source);
    advance(); // current: garbage next: 3
    AST *ast = block_stmt();
    return ast;
}