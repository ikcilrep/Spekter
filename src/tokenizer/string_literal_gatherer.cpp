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
        add_character_to_string_literal(text);
        iterator->next_character();
    }

    return text;
}

void string_literal_gatherer::add_character_to_string_literal(std::string& text) {
    if (iterator->current_character == '\\')
        text += escape_parser.parse_escape_sequence();
    else
        text += iterator->current_character.value();

}