#ifndef unit_test 
#define unit_test
#include "../include/acutest.h"

// AST Tests
void should_generate_ast(void);

// Lexer Tests
void should_identify_single_character_tokens(void);
void should_increment_line_number_with_comments(void);
void should_identify_single_or_double_character_minus_tokens(void);
void should_identify_single_or_double_character_plus_tokens(void);
void should_identify_single_or_double_character_slash_tokens(void);
void should_identify_single_or_double_character_star_tokens(void);
void should_identify_single_or_double_character_and_tokens(void);
void should_identify_single_or_double_character_or_tokens(void);
void should_identify_single_or_double_character_equal_tokens(void);
void should_identify_single_or_double_character_greater_tokens(void);
void should_identify_single_or_double_character_less_tokens(void);
void should_identify_type_tokens(void);
void should_identify_values_types(void);
void should_identify_reserved_words(void);
void should_identify_identifier(void);

#endif