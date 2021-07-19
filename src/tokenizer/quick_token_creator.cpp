#include "quick_token_creator.h"

using namespace spekter;

quick_token_creator::quick_token_creator(std::shared_ptr<character_iterator> iterator) : iterator(iterator) {}
quick_token_creator::quick_token_creator() {}


token quick_token_creator::create_token(token_type type) {
    return token(type, iterator->line_number, iterator->char_in_line_number, iterator->char_number);
}

token quick_token_creator::create_token(token_type type, std::string text) {
    return token(type, iterator->line_number, iterator->char_in_line_number, iterator->char_number, text);
}

