#ifndef tokenizer_h
#define tokenizer_h

#include <memory>
#include <istream>
#include <unordered_map>
#include <functional>
#include <cuchar>
#include "tokens/token.h"
#include "character_iterator.h"



namespace spekter
{
    class tokenizer
    {
        token create_token(token_type type);
        token create_token(token_type type, std::string text);

        std::string gather_characters(std::function<bool(char)> is_in_group,
                std::function<bool(std::string)> is_finished = [](auto _) {return false;});
               
        std::string gather_string_literal_characters();

        void add_character_to_string_literal(std::string& next_token_text); 

        std::optional<token> lazy_next_token;
        static const std::unordered_map<std::string, token_type> constant_text_to_token_type;
        static const std::unordered_map<char, char32_t> escapable_characters_to_escaped;

        character_iterator iterator; 
        token get_token_with_constant_text(const std::string& text);

        char32_t parse_escaped_character();
        char32_t parse_unicode_character();
        char32_t parse_unicode_character_code(const std::string &unicode_character_code_text);

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
