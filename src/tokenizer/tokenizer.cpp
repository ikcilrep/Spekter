#include "tokenizer.h"
#include "tokens/token.h"
#include <locale>

using namespace spekter;

const std::unordered_map<std::string, token_type> tokenizer::constant_text_to_token_type = {
    {"(", token_type::PARENTHESIS_OPEN},
    {")", token_type::PARENTHESIS_CLOSE},
    {"[", token_type::SQUARE_BRACKET_OPEN},
    {"]", token_type::SQUARE_BRACKET_CLOSE},
    {"{", token_type::CURLY_BRACKET_OPEN},
    {"}", token_type::CURLY_BRACKET_CLOSE},
    {"->", token_type::ARROW_OPERATOR},
    {"+", token_type::PLUS_OPERATOR},
    {"-", token_type::MINUS_OPERATOR},
    {"*", token_type::TIMES_OPERATOR},
    {"/", token_type::DIVIDE_OPERATOR},
    {"**", token_type::POWER_OPERATOR},
    {"&", token_type::AND_OPERATOR},
    {"|", token_type::OR_OPERATOR},
    {"^", token_type::XOR_OPERATOR},
    {"!", token_type::NEGATION_OPERATOR},
    {".", token_type::DOT_OPERATOR},
    {"==", token_type::EQUALS_OPERATOR},
    {"!=", token_type::NOT_EQUALS_OPERATOR},
    {">", token_type::GREATER_THAN_OPERATOR},
    {"<", token_type::LESS_THAN_OPERATOR},
    {">=", token_type::GREATER_THAN_OR_EQUAL_TO_OPERATOR},
    {"<=", token_type::LESS_THAN_OR_EQUAL_TO_OPERATOR},
    {"=", token_type::ASSIGNMENT_OPERATOR},
    {"+=", token_type::PLUS_ASSIGNMENT_OPERATOR},
    {"-=", token_type::MINUS_ASSIGNMENT_OPERATOR},
    {"*=", token_type::TIMES_ASSIGNMENT_OPERATOR},
    {"/=", token_type::DIVIDE_ASSIGNMENT_OPERATOR},
    {"**=", token_type::POWER_ASSIGNMENT_OPERATOR},
    {"&=", token_type::AND_ASSIGNMENT_OPERATOR},
    {"|=", token_type::OR_ASSIGNMENT_OPERATOR},
    {"^=", token_type::XOR_ASSIGNMENT_OPERATOR},
    {"try", token_type::TRY_KEYWORD},
    {"catch", token_type::CATCH_KEYWORD},
    {"import", token_type::IMPORT_KEYWORD},
    {"from", token_type::FROM_KEYWORD},
    {"if", token_type::IF_KEYWORD},
    {"unless", token_type::UNLESS_KEYWORD},
    {"while", token_type::WHILE_KEYWORD},
    {"for", token_type::FOR_KEYWORD},
    {"loop", token_type::LOOP_KEYWORD},
    {"let", token_type::LET_KEYWORD},
    {"async", token_type::ASYNC_KEYWORD},
    {"await", token_type::AWAIT_KEYWORD},
    {"yield", token_type::YIELD_KEYWORD},
    {"break", token_type::BREAK_KEYWORD},
    {"continue", token_type::CONTINUE_KEYWORD},
    {"as", token_type::AS_KEYWORD},
    {"gpu", token_type::GPU_KEYWORD},
    {"return", token_type::RETURN_KEYWORD},
    {"defer", token_type::DEFER_KEYWORD},
    {"match", token_type::MATCH_KEYWORD},
    {"enum", token_type::ENUM_KEYWORD},
    {"class", token_type::CLASS_KEYWORD},
    {"interface", token_type::INTERFACE_KEYWORD},
    {"abstract", token_type::ABSTRACT_KEYWORD},
    {"mut", token_type::MUT_KEYWORD},
    {"boolean", token_type::BOOLEAN_KEYWORD},
    {"string", token_type::STRING_KEYWORD},
    {"char", token_type::CHAR_KEYWORD},
    {"int8", token_type::INT8_KEYWORD},
    {"int16", token_type::INT16_KEYWORD},
    {"int32", token_type::INT32_KEYWORD},
    {"int64", token_type::INT64_KEYWORD},
    {"uint8", token_type::UINT8_KEYWORD},
    {"uint16", token_type::UINT16_KEYWORD},
    {"uint32", token_type::UINT32_KEYWORD},
    {"uint64", token_type::UINT64_KEYWORD},
    {"float32", token_type::FLOAT32_KEYWORD},
    {"float64", token_type::FLOAT64_KEYWORD},
    {"void", token_type::VOID_KEYWORD},
    {"public", token_type::PUBLIC_KEYWORD},
    {"private", token_type::PRIVATE_KEYWORD},
    {"protected", token_type::PROTECTED_KEYWORD},
    {"true", token_type::BOOLEAN_LITERAL},
    {"false", token_type::BOOLEAN_LITERAL},
};

tokenizer::tokenizer(std::unique_ptr<std::istream> code)
{
    this->code = std::move(code);
    next_character();
}

std::string tokenizer::gather_characters(std::function<bool(char)> is_in_group) {
    std::string text = "";
    do {
        text += current_character.value();
        next_character();
    } while (current_character.has_value() && is_in_group(current_character.value()));
    return text;
}

token tokenizer::get_token_with_constant_text(const std::string& text) {
    return token(tokenizer::constant_text_to_token_type.at(text), line_number, char_in_line_number, char_number);
}

void tokenizer::update_line_number(char character) {
    if (character == '\n') {
        line_number += 1;
        char_in_line_number = 0;
    }
    else {
        char_in_line_number += 1;
    }
}

void tokenizer::next_character() {
    char character;
    if ((*this->code) >> std::noskipws >> character) {
        char_number += 1;
        update_line_number(character);
        current_character = character;
    }
    else {
        current_character.reset();
    }
}

std::optional<token> tokenizer::tokenize_alphanumeric() {
    std::string next_token_text = gather_characters(isalnum);
    if (constant_text_to_token_type.contains(next_token_text)) {
        return get_token_with_constant_text(next_token_text);
    }

    return token(token_type::IDENTIFIER, line_number, char_in_line_number, char_number, next_token_text);
}

std::optional<token> tokenizer::handle_dot_after_digit_sequence(std::string next_token_text) {
    if (current_character == '.') {
        next_character();
        if (isdigit(current_character.value())) {
            return tokenize_float_literal(next_token_text);
        }
        else {
            lazy_next_token = token(token_type::DOT_OPERATOR, line_number, char_in_line_number, char_number);
        }
    }
    return {};
}

std::optional<token> tokenizer::tokenize_float_literal(std::string next_token_text) {
    next_token_text += '.';
    next_token_text += gather_characters(isdigit);
    return token(token_type::FLOAT_LITERAL, line_number, char_in_line_number, char_number, next_token_text);
}

std::optional<token> tokenizer::tokenize_number_literal() {
    std::string next_token_text = gather_characters(isdigit);

    auto float_literal_token = handle_dot_after_digit_sequence(next_token_text);
    if (float_literal_token.has_value()) {
        return float_literal_token;
    }

    return token(token_type::INT_LITERAL, line_number, char_in_line_number, char_number, next_token_text);
}


std::optional<token> tokenizer::next_token() {
    if (lazy_next_token.has_value()) {
        lazy_next_token.reset();
        return lazy_next_token;
    }

    if (current_character.has_value()) {
        if (isalpha(current_character.value())) {
            return tokenize_alphanumeric();
        }
        else if (isdigit(current_character.value())) {
            return tokenize_number_literal();
        }
    }

    return {};
}
