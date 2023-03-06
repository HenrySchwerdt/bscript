#include "test.h"
#include "bs_ast_unit.h"
#include "bs_lex_unit.h"
#include "bs_parser_unit.h"

TEST_LIST = {
    {": Should generate ast", should_generate_ast},
    {": Should increment line numbers in comments", should_increment_line_number_with_comments},
    {": Should identify '(){}[],.~:;?!' tokens", should_identify_single_character_tokens},
    {": Should identify '- -= --' tokens", should_identify_single_or_double_character_minus_tokens},
    {": Should identify '+ += ++' tokens", should_identify_single_or_double_character_plus_tokens},
    {": Should identify '/ /=' tokens", should_identify_single_or_double_character_slash_tokens},
    {": Should identify '* *=' tokens", should_identify_single_or_double_character_star_tokens},
    {": Should identify '& && &=' tokens", should_identify_single_or_double_character_and_tokens},
    {": Should identify '| || |=' tokens", should_identify_single_or_double_character_or_tokens},
    {": Should identify '= == =>' tokens", should_identify_single_or_double_character_equal_tokens},
    {": Should identify '> >> >=' tokens", should_identify_single_or_double_character_greater_tokens},
    {": Should identify '< << <=' tokens", should_identify_single_or_double_character_less_tokens},
    {": Should identify primitive type tokens", should_identify_type_tokens},
    {": Should identify value types tokens", should_identify_values_types},
    {": Should identify reserved words", should_identify_reserved_words},
    {": Should identify identifiers", should_identify_identifier},
    {": Should parse binary expressions", should_parse_binary_expressions},
    {": Should parse complex binary expressions", should_parse_complex_expressions},
    {": Should parse complex binary expressions", should_parse_expressions},
    {": Should parse block stmt", should_parse_block},
    {NULL, NULL}
};