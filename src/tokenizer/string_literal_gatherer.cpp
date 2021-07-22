#include "string_literal_gatherer.h"

using namespace spekter;

string_literal_gatherer::string_literal_gatherer() {}
string_literal_gatherer::string_literal_gatherer(std::shared_ptr<character_iterator> iterator) {
    this->iterator = iterator;
    escape_parser = escape_sequence_parser(iterator);
}

std::string string_literal_gatherer::gather_characters() {
    std::string text = "";

    iterator->next_character();
    while (iterator->current_character != '\"') {
        if (iterator->current_character == '\\') {
            text += escape_parser.parse_escape_sequence();
        }
        else {
            text += iterator->current_character.value();
        }
        iterator->next_character();
    }

    return text;
}