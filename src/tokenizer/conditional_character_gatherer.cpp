#include "conditional_character_gatherer.h"

using namespace spekter;

conditional_character_gatherer::conditional_character_gatherer() {}

conditional_character_gatherer::conditional_character_gatherer(std::shared_ptr<character_iterator> iterator)
    : iterator(iterator) {}

std::string conditional_character_gatherer::gather_characters(std::function<bool(char)> is_in_group, std::function<bool(std::string)> is_finished) {
    std::string text = "";
    do {
        text += iterator->current_character.value();
        iterator->next_character();
    } while (iterator->current_character.has_value() && is_in_group(iterator->current_character.value()) && !is_finished(text));
    return text;
}




