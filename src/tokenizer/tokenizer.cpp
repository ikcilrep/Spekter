#include "tokenizer.h"
#include "tokens/token.h"
#include <locale>
#include <stdexcept>

using namespace spekter;

tokenizer::tokenizer(std::unique_ptr<std::istream> code)
{
    iterator = make_shared<character_iterator>(std::move(code));
    conditional_gatherer = conditional_character_gatherer(iterator);
    string_gatherer = string_literal_gatherer(iterator);
    token_creator = quick_token_creator(iterator);
    iterator->next_character();
}

std::optional<token> tokenizer::next_token() {
    if (lazy_next_token.has_value())
        return get_lazy_next_token();

    if (iterator->current_character.has_value())
        return tokenize_further();

    return {};
}

std::optional<token> tokenizer::get_lazy_next_token() {
    lazy_next_token.reset();
    return lazy_next_token;
}

token tokenizer::tokenize_further() {
    iterator->skip_white_spaces(); 

    if (isalpha(iterator->current_character.value()))
        return tokenize_alphanumeric();
    else if (isdigit(iterator->current_character.value()))
        return tokenize_number_literal();
    else if (ispunct(iterator->current_character.value()))
        return tokenize_operators_and_symbols();
    else if (iterator->current_character.value() == '\"')
        return tokenize_string_literal();

    //temporary
    return token_creator.create_token(token_type::UNKNOWN);
}

token tokenizer::tokenize_string_literal() {
    return token_creator.create_token(token_type::STRING_LITERAL, string_gatherer.gather_characters());
}

token tokenizer::tokenize_operators_and_symbols() {
    auto is_finished = [](std::string text) {return quick_token_creator::is_text_constant(text);};
    std::string next_token_text = conditional_gatherer.gather_characters(ispunct, is_finished);
    if (quick_token_creator::is_text_constant(next_token_text))
        return token_creator.create_token_with_constant_text(next_token_text);


    return token_creator.create_token(token_type::UNKNOWN, next_token_text);
}

token tokenizer::tokenize_alphanumeric() {
    std::string next_token_text = conditional_gatherer.gather_characters(isalnum);
    if (quick_token_creator::is_text_constant(next_token_text))
        return token_creator.create_token_with_constant_text(next_token_text);

    return token_creator.create_token(token_type::IDENTIFIER, next_token_text);
}

token tokenizer::tokenize_number_literal() {
    std::string next_token_text = conditional_gatherer.gather_characters(isdigit);

    auto float_literal_token = handle_dot_after_digit_sequence(next_token_text);
    if (float_literal_token.has_value())
        return float_literal_token.value();

    return token_creator.create_token(token_type::INT_LITERAL, next_token_text);
}

std::optional<token> tokenizer::handle_dot_after_digit_sequence(std::string next_token_text) {
    if (iterator->current_character == '.') {
        iterator->next_character();
        if (isdigit(iterator->current_character.value()))
            return tokenize_float_literal(next_token_text);
        else
            lazy_next_token = token_creator.create_token(token_type::DOT_OPERATOR);
    }
    return {};
}

token tokenizer::tokenize_float_literal(std::string next_token_text) {
    next_token_text += '.';
    next_token_text += conditional_gatherer.gather_characters(isdigit);
    return token_creator.create_token(token_type::FLOAT_LITERAL, next_token_text);
}