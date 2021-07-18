#include "tokenizer.h"
#include "tokens/token.h"
#include <locale>
#include <stdexcept>

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

const std::unordered_map<char, char32_t> escapable_characters = {
    {'\'', '\''},
    {'\"', '\"'},
    {'\\', '\\'},
    {'a', '\a'},
    {'b', '\b'},
    {'f', '\f'},
    {'n', '\n'},
    {'r', '\r'},
    {'t', '\t'},
    {'v', '\v'} };


tokenizer::tokenizer(std::unique_ptr<std::istream> code)
{
    iterator = character_iterator(std::move(code));
    iterator.next_character();
}

token tokenizer::create_token(token_type type) {
    return token(type, iterator.line_number, iterator.char_in_line_number, iterator.char_number);
}

token tokenizer::create_token(token_type type, std::string text) {
    return token(type, iterator.line_number, iterator.char_in_line_number, iterator.char_number, text);
}

std::optional<token> tokenizer::next_token() {
    if (lazy_next_token.has_value())
        return get_lazy_next_token();

    if (iterator.current_character.has_value())
        return tokenize_further();

    return {};
}

std::optional<token> tokenizer::get_lazy_next_token() {
    lazy_next_token.reset();
    return lazy_next_token;
}

token tokenizer::tokenize_further() {
    if (isalpha(iterator.current_character.value()))
        return tokenize_alphanumeric();
    else if (isdigit(iterator.current_character.value()))
        return tokenize_number_literal();
    else if (ispunct(iterator.current_character.value()))
        return tokenize_operators_and_symbols();
    else if (iterator.current_character.value() == '\\')
        return tokenize_string_literal();

    //temporary
    return create_token(token_type::UNKNOWN);
}

char32_t tokenizer::parse_unicode_character_code(const std::string& unicode_character_code_text) {
    try {
        return std::stoi(unicode_character_code_text, 0, 16);
    }
    catch (const std::invalid_argument& exception) {
        throw std::invalid_argument(
            std::string("The escaped character code \'") + unicode_character_code_text + "\' is not hex value.");
    }
}


char32_t tokenizer::parse_unicode_character() {
    iterator.next_character();
    if (iterator.current_character != '{')
        throw new std::invalid_argument(
            std::string("Invalid character to escape \'\\") + iterator.current_character.value() + "u\'.");

    std::string unicode_character_code_text =
        gather_characters([](char character) {return isxdigit(character) && character != '}';});

    return parse_unicode_character_code(unicode_character_code_text);

}

char32_t tokenizer::parse_escaped_character() {
    if (escapable_characters.contains(iterator.current_character.value()))
        return escapable_characters.at(iterator.current_character.value());
    else if (iterator.current_character == 'u')
        return parse_unicode_character();
    else
        throw new std::invalid_argument(std::string("Invalid character to escape \'\\") + iterator.current_character.value() + "\'.");
}

std::string tokenizer::gather_string_literal_characters() {
    std::string text = "";
    iterator.next_character();
    while (iterator.current_character != '\"') {
        iterator.next_character();
        if (iterator.current_character == '\\') {
            text += parse_escaped_character();
        }
        else {
            text += iterator.current_character.value();
        }
        iterator.next_character();
    }

    return text;
}

token tokenizer::tokenize_string_literal() {
    return create_token(token_type::STRING_LITERAL, gather_string_literal_characters());
}

token tokenizer::tokenize_operators_and_symbols() {
    auto is_finished = [](std::string text) {return constant_text_to_token_type.contains(text);};
    std::string next_token_text = gather_characters(ispunct, is_finished);
    if (constant_text_to_token_type.contains(next_token_text))
        return get_token_with_constant_text(next_token_text);


    return create_token(token_type::UNKNOWN, next_token_text);
}

token tokenizer::tokenize_alphanumeric() {
    std::string next_token_text = gather_characters(isalnum);
    if (constant_text_to_token_type.contains(next_token_text))
        return get_token_with_constant_text(next_token_text);

    return create_token(token_type::IDENTIFIER, next_token_text);
}

token tokenizer::get_token_with_constant_text(const std::string& text) {
    return create_token(tokenizer::constant_text_to_token_type.at(text));
}

token tokenizer::tokenize_number_literal() {
    std::string next_token_text = gather_characters(isdigit);

    auto float_literal_token = handle_dot_after_digit_sequence(next_token_text);
    if (float_literal_token.has_value())
        return float_literal_token.value();

    return create_token(token_type::INT_LITERAL, next_token_text);
}

std::optional<token> tokenizer::handle_dot_after_digit_sequence(std::string next_token_text) {
    if (iterator.current_character == '.') {
        iterator.next_character();
        if (isdigit(iterator.current_character.value()))
            return tokenize_float_literal(next_token_text);
        else
            lazy_next_token = create_token(token_type::DOT_OPERATOR);
    }
    return {};
}

token tokenizer::tokenize_float_literal(std::string next_token_text) {
    next_token_text += '.';
    next_token_text += gather_characters(isdigit);
    return create_token(token_type::FLOAT_LITERAL, next_token_text);
}

std::string tokenizer::gather_characters(std::function<bool(char)> is_in_group, std::function<bool(std::string)> is_finished) {
    std::string text = "";
    do {
        text += iterator.current_character.value();
        iterator.next_character();
    } while (iterator.current_character.has_value() && is_in_group(iterator.current_character.value()) && !is_finished(text));
    return text;
}



