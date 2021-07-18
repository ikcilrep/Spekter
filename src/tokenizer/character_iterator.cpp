#include "character_iterator.h"

using namespace spekter;

character_iterator::character_iterator() {}

character_iterator::character_iterator(std::unique_ptr<std::istream> character_stream) {
    this->character_stream = std::move(character_stream);
}

void character_iterator::next_character() {
    char character;
    if ((*this->character_stream) >> std::noskipws >> character)
        set_current_character(character);
    else
        current_character.reset();
}

void character_iterator::set_current_character(char character) {
    char_number += 1;
    update_line_number(character);
    current_character = character;
}

void character_iterator::update_line_number(char character) {
    if (character == '\n')
        increment_line_number();
    else
        char_in_line_number += 1;
}

void character_iterator::increment_line_number() {
    line_number += 1;
    char_in_line_number = 0;
}