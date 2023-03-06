#include <stdio.h>
#include "bs_ast.h"

AST *ast_new(AST ast)
{
  AST *ptr = malloc(sizeof(AST));
  if (ptr)
    *ptr = ast;
  return ptr;
}

void ast_free(AST *ptr)
{
  AST ast = *ptr;
  switch (ast.type)
  {
  case LITERAL:
  {
    free_value(ast.data.LITERAL.value);
    break;
  }
  case IDENTIFIER:
  {
    break;
  }
  case UNARY_EXPRESSION:
  {
    struct UNARY_EXPRESSION data = ast.data.UNARY_EXPRESSION;
    ast_free(data.argument);
    break;
  }
  case BINARY_EXPRESSION:
  {
    struct BINARY_EXPRESSION data = ast.data.BINARY_EXPRESSION;
    ast_free(data.left);
    ast_free(data.right);
    break;
  }
  case CONDITIONAL_EXPRESSION:
  {
    struct CONDITIONAL_EXPRESSION data = ast.data.CONDITIONAL_EXPRESSION;
    ast_free(data.test);
    ast_free(data.consequent);
    ast_free(data.alternate);
    break;
  }
  case EXPRESSION_STATEMENT:
  {
    struct EXPRESSION_STATEMENT data = ast.data.EXPRESSION_STATEMENT;
    ast_free(data.expression);
    break;
  }
  case VARIABLE_DECLARATOR:
  {
    struct VARIABLE_DECLARATOR data = ast.data.VARIABLE_DECLARATOR;
    ast_free(data.id);
    ast_free(data.init);
    break;
  }
  case VARIABLE_DECLARATION:
  {
    struct VARIABLE_DECLARATION data = ast.data.VARIABLE_DECLARATION;
    ast_free(data.declaration);
    break;
  }
  case BLOCK_STMT:
  {
    struct BLOCK_STMT data = ast.data.BLOCK_STMT;
    for (int i = 0; i < data.instructionCount; i++)
    {
      ast_free(data.instructions[i]);
    }
    break;
  }
  }
  free(ptr);
}

static char *operator_str(ast_operator operator)
{
  switch (operator)
  {
  case BIT_NOT:
    return "\"~\"";
  case BANG:
    return "\"!\"";
  case MINUS:
    return "\"-\"";
  case MINUS_MINUS:
    return "\"--\"";
  case PLUS:
    return "\"+\"";
  case PLUS_PLUS:
    return "\"++\"";
  case SLASH:
    return "\"/\"";
  case STAR:
    return "\"*\"";
  case BIT_AND:
    return "\"&\"";
  case BIT_OR:
    return "\"|\"";
  case EQUAL_EQUAL:
    return "\"==\"";
  case GREATER:
    return "\">\"";
  case BIT_SHIFT_RIGHT:
    return "\">>\"";
  case GREATER_EQUAL:
    return "\">=\"";
  case LESS:
    return "\"<\"";
  case BIT_SHIFT_LEFT:
    return "\"<<\"";
  case LESS_EQUAL:
    return "\"<=\"";
  default:
    return "Operator Invalid";
  }
}

static void print_tabs(int depth, FILE *output)
{
  for (int i = 0; i < depth; i++)
    fprintf(output, "    ");
}

static void print_type(ast_type type, int depth, FILE *output)
{
  fprintf(output, "{\n");
  print_tabs(depth, output);
  fprintf(output, "\"type\": ");
  switch (type)
  {
  case LITERAL:
    fprintf(output, "\"literal\",\n");
    break;
  case IDENTIFIER:
    fprintf(output, "\"identifier\",\n");
    break;
  case UNARY_EXPRESSION:
    fprintf(output, "\"unary-expression\",\n");
    break;
  case BINARY_EXPRESSION:
    fprintf(output, "\"binary-expression\",\n");
    break;
  case CONDITIONAL_EXPRESSION:
    fprintf(output, "\"conditional-expression\",\n");
    break;
  case EXPRESSION_STATEMENT:
    fprintf(output, "\"expression-statement\",\n");
    break;
  case VARIABLE_DECLARATION:
    fprintf(output, "\"variable-declaration\",\n");
    break;
  case VARIABLE_DECLARATOR:
    fprintf(output, "\"variable-declarator\",\n");
    break;
  case BLOCK_STMT:
    fprintf(output, "\"block-stmt\",\n");
    break;
  default:
    break;
  }
}

static char *get_type_str(bs_value_type type)
{
  switch (type)
  {
  case BS_BOOL:
    return "bool";
  case BS_INT8:
    return "int8";
  case BS_UINT8:
    return "u_int8";
  case BS_INT16:
    return "int16";
  case BS_UINT16:
    return "u_int16";
  case BS_INT32:
    return "int32";
  case BS_UINT32:
    return "u_int32";
  case BS_INT64:
    return "int64";
  case BS_UINT64:
    return "u_int64";
  }
  return "unknown";
}

static void print_literal(bs_value *literal_value, int depth, FILE *output)
{
  fprintf(output, "\"data-type\": \"%s\",\n", get_type_str(literal_value->type));
  print_tabs(depth, output);
  switch (literal_value->type)
  {
  case BS_BOOL:
    fprintf(output, "\"value\": %d\n", AS_CVAL(literal_value, BS_BOOL));
    break;
  case BS_INT8:
    fprintf(output, "\"value\": %d\n", AS_CVAL(literal_value, BS_INT8));
    break;
  case BS_UINT8:
    fprintf(output, "\"value\": %d\n", AS_CVAL(literal_value, BS_UINT8));
    break;
  case BS_INT16:
    fprintf(output, "\"value\": %d\n", AS_CVAL(literal_value, BS_INT16));
    break;
  case BS_UINT16:
    fprintf(output, "\"value\": %d\n", AS_CVAL(literal_value, BS_UINT16));
    break;
  case BS_INT32:
    fprintf(output, "\"value\": %d\n", AS_CVAL(literal_value, BS_INT32));
    break;
  case BS_UINT32:
    fprintf(output, "\"value\": %d\n", AS_CVAL(literal_value, BS_UINT32));
    break;
  case BS_INT64:
    fprintf(output, "\"value\": %d\n", AS_CVAL(literal_value, BS_INT64));
    break;
  case BS_UINT64:
    fprintf(output, "\"value\": %d\n", AS_CVAL(literal_value, BS_UINT64));
    break;
  default:
    break;
  }
}

static void ast_print_depth(AST *ast, int depth, bool is_last, FILE *output)
{
  ast_type current_type = ast->type;
  print_type(current_type, depth + 1, output);
  print_tabs(depth + 1, output);
  switch (current_type)
  {
  case LITERAL:
    print_literal(AST_DATA(ast, LITERAL).value, depth + 1, output);
    break;
  case IDENTIFIER:
    fprintf(output, "\"name\": \"%.*s\"\n", AST_DATA(ast, IDENTIFIER).length, AST_DATA(ast, IDENTIFIER).name);
    break;
  case UNARY_EXPRESSION:
    fprintf(output, "\"operator\": %s,\n", operator_str(AST_DATA(ast, UNARY_EXPRESSION).operator));
    print_tabs(depth + 1, output);
    fprintf(output, "\"argument\": ");
    ast_print_depth(AST_DATA(ast, UNARY_EXPRESSION).argument, depth + 1, true, output);
    break;
  case BINARY_EXPRESSION:
    fprintf(output, "\"operator\": %s,\n", operator_str(AST_DATA(ast, BINARY_EXPRESSION).operator));
    print_tabs(depth + 1, output);
    fprintf(output, "\"left\": ");
    ast_print_depth(AST_DATA(ast, BINARY_EXPRESSION).left, depth + 1, false, output);
    print_tabs(depth + 1, output);
    fprintf(output, "\"right\": ");
    ast_print_depth(AST_DATA(ast, BINARY_EXPRESSION).right, depth + 1, true, output);
    break;
  case VARIABLE_DECLARATION:
    fprintf(output, "\"kind\": \"%s\",\n", AST_DATA(ast, VARIABLE_DECLARATION).kind == VAL ? "val" : "var");
    print_tabs(depth + 1, output);
    fprintf(output, "\"valueType\": \"%s\",\n", get_type_str(AST_DATA(ast, VARIABLE_DECLARATION).type));
    print_tabs(depth + 1, output);
    fprintf(output, "\"declaration\": ");
    ast_print_depth(AST_DATA(ast, VARIABLE_DECLARATION).declaration, depth + 1, true, output);
    break;
  case VARIABLE_DECLARATOR:
    fprintf(output, "\"id\": ");
    ast_print_depth(AST_DATA(ast, VARIABLE_DECLARATOR).id, depth + 1, false, output);
    print_tabs(depth + 1, output);
    fprintf(output, "\"init\": ");
    if (AST_DATA(ast, VARIABLE_DECLARATOR).init == NULL)
    {
      fprintf(output, "null\n");
    }
    else
    {
      ast_print_depth(AST_DATA(ast, VARIABLE_DECLARATOR).init, depth + 1, true, output);
    }
    break;
  case BLOCK_STMT:
    fprintf(output, "\"instructionCount\": %d,\n", AST_DATA(ast, BLOCK_STMT).instructionCount);
    print_tabs(depth + 1, output);
    fprintf(output, "\"instructions\": [");

    for (int i = 0; i < AST_DATA(ast, BLOCK_STMT).instructionCount; i++)
    {
      print_tabs(depth + 1, output);
      ast_print_depth(AST_DATA(ast, BLOCK_STMT).instructions[i], depth + 1, i == AST_DATA(ast, BLOCK_STMT).instructionCount - 1 ? true : false, output);
    }
    print_tabs(depth + 1, output);
    fprintf(output, "]\n");
  }
  print_tabs(depth, output);
  fprintf(output, "}%c\n", is_last ? ' ' : ',');
}

void ast_print(AST *ast, FILE *output)
{
  ast_print_depth(ast, 0, true, output);
}