#include "test.h"
#include "../src/bs_parser.h"
#include "../src/bs_value.h"

void should_parse_binary_expressions(void)
{
    // given
    const char *source = "3 + 4 * 5";
    // when
    AST *ast = parse(source);
    // then
    TEST_CHECK(ast->type == BINARY_EXPRESSION);
    TEST_CHECK(AST_DATA(ast, BINARY_EXPRESSION).operator== PLUS);
    AST *top_left = AST_DATA(ast, BINARY_EXPRESSION).left;
    TEST_CHECK(top_left->type == LITERAL);
    bs_value *literal1 = AST_DATA(top_left, LITERAL).value;
    TEST_CHECK(IS_TYPE(literal1, BS_INT32));
    TEST_CHECK(AS_CVAL(literal1, BS_INT32) == 3);
    AST *top_right = AST_DATA(ast, BINARY_EXPRESSION).right;
    TEST_CHECK(top_right->type == BINARY_EXPRESSION);
    TEST_CHECK(AST_DATA(top_right, BINARY_EXPRESSION).operator== STAR);
    AST *left = AST_DATA(top_right, BINARY_EXPRESSION).left;
    bs_value *literal2 = AST_DATA(left, LITERAL).value;
    TEST_CHECK(IS_TYPE(literal2, BS_INT32));
    TEST_CHECK(AS_CVAL(literal2, BS_INT32) == 4);
    AST *right = AST_DATA(top_right, BINARY_EXPRESSION).right;
    bs_value *literal3 = AST_DATA(right, LITERAL).value;
    TEST_CHECK(IS_TYPE(literal3, BS_INT32));
    TEST_CHECK(AS_CVAL(literal3, BS_INT32) == 5);
    printf("\n");
    ast_print(ast, stdout);
    ast_free(ast);
}

void should_parse_complex_expressions(void)
{
    // given
    // TODO post -- ++ doenst work
    const char *source = "3 << 2 & 4 | 5 * ~10";
    // when
    AST *ast = parse(source);
    // then
    TEST_CHECK(ast->type == BINARY_EXPRESSION);
    TEST_CHECK(AST_DATA(ast, BINARY_EXPRESSION).operator== BIT_OR);
    printf("\n");
    ast_print(ast, stdout);
    ast_free(ast);
}

void should_parse_expressions(void)
{
    // given
    // TODO post -- ++ doenst work
    const char *source = "val x;";
    // when
    AST *ast = parse(source);
    // then
    // TEST_CHECK(ast->type == BINARY_EXPRESSION);
    // TEST_CHECK(AST_DATA(ast, BINARY_EXPRESSION).operator == BIT_OR);
    printf("\n");
    ast_print(ast, stdout);
    // ast_free(ast);
}

void should_parse_block(void)
{
    // given
    // TODO post -- ++ doenst work
    const char *source = "{var x: int8 = 10;var y = 12;val z = x + y;}";
    // when
    AST *ast = parse(source);
    // then
    // TEST_CHECK(ast->type == BINARY_EXPRESSION);
    // TEST_CHECK(AST_DATA(ast, BINARY_EXPRESSION).operator == BIT_OR);
    printf("\n");
    ast_print(ast, stdout);
    // ast_free(ast);
}
