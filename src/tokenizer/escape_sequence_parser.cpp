#include "escape_sequence_parser.h"

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


escape_sequence_parser::escape_sequence_parser() {}

escape_sequence_parser::escape_sequence_parser(std::shared_ptr<character_iterator> iterator) {
    this->iterator = iterator;
    conditional_gatherer = conditional_character_gatherer(iterator);
}

char32_t escape_sequence_parser::parse_unicode_character_code(const std::string& unicode_character_code_text) {
    try {
        return std::stoi(unicode_character_code_text, 0, 16);
    }
    catch (const std::invalid_argument& exception) {
        throw std::invalid_argument(
            std::string("The escaped character code \'") + unicode_character_code_text + "\' is not hex value.");
    }
}


char32_t escape_sequence_parser::parse_unicode_escape_sequence() {
    iterator->next_character();
    if (iterator->current_character != '{')
        throw new std::invalid_argument(
            std::string("Invalid character to escape \'\\") + iterator->current_character.value() + "u\'.");

    std::string unicode_character_code_text =
        conditional_gatherer.gather_characters([](char character) {return isxdigit(character) && character != '}';});

    return parse_unicode_character_code(unicode_character_code_text);

}

char32_t escape_sequence_parser::parse_escape_sequence() {
    if (escapable_characters.contains(iterator->current_character.value()))
        return escapable_characters.at(iterator->current_character.value());
    else if (iterator->current_character == 'u')
        return parse_unicode_escape_sequence();
    else
        throw new std::invalid_argument(std::string("Invalid character to escape \'\\") + iterator->current_character.value() + "\'.");
}