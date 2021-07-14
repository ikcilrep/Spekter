#ifndef tokenizer_h
#define tokenizer_h

#include <memory>
#include <istream>
#include <unordered_map>
#include <unordered_set>
#include "tokens/token.h"



namespace spekter
{
    class tokenizer
    {
        template<typename Function>
        void gather_characters(Function is_in_group);

        std::unique_ptr<std::istream> code;
        std::optional<char> current_character;
        std::optional<token> lazy_next_token;
        std::string next_token_text = "";
        int char_number = 0;
        int line_number = 0;
        int char_in_line_number = 0;
        static const std::unordered_map<std::string, token_type> constant_text_to_token_type;

        void next_character();
        void update_line_number(char character);
        token get_token_with_constant_text(const std::string& text);


        std::optional<token> tokenize_alphanumeric();
        std::optional<token> tokenize_number_literal();
        std::optional<token> handle_dot_after_digit_sequence();
        std::optional<token> tokenize_float_literal();

    public:
        tokenizer(std::unique_ptr<std::istream> code);
        std::optional<token> next_token();
    };
}

#endif
