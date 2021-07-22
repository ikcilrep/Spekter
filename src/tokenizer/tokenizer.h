#ifndef tokenizer_h
#define tokenizer_h

#include <memory>
#include <istream>
#include <unordered_map>
#include <functional>
#include <cuchar>
#include "tokens/token.h"
#include "character_iterator.h"
#include "escape_sequence_parser.h"
#include "conditional_character_gatherer.h"
#include "string_literal_gatherer.h"
#include "quick_token_creator.h"



namespace spekter
{
    class tokenizer
    {
        quick_token_creator token_creator;

        conditional_character_gatherer conditional_gatherer;

        string_literal_gatherer string_gatherer;


        std::optional<token> lazy_next_token;

        std::shared_ptr<character_iterator> iterator;


        token tokenize_further();
        token tokenize_operators_and_symbols();
        token tokenize_alphanumeric();
        token tokenize_number_literal();
        token tokenize_string_literal();
        std::optional<token> handle_dot_after_digit_sequence(std::string next_token_text);
        token tokenize_float_literal(std::string next_token_text);

        std::optional<token> get_lazy_next_token();

    public:
        tokenizer(std::unique_ptr<std::istream> code);
        std::optional<token> next_token();
    };
}

#endif
