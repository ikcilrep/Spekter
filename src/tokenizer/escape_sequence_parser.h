#ifndef escape_sequence_parser_h
#define escape_sequence_parser_h

#include <memory>
#include <unordered_map>
#include "character_iterator.h"
#include "conditional_character_gatherer.h"

using namespace spekter;

class escape_sequence_parser {

    std::shared_ptr<character_iterator> iterator;
    conditional_character_gatherer conditional_gatherer;

    char32_t parse_unicode_escape_sequence();
    char32_t parse_unicode_character_code(const std::string& unicode_character_code_text);

    static const std::unordered_map<char, char32_t> escapable_characters_to_escaped;

public:
    char32_t parse_escape_sequence();
    escape_sequence_parser();
    escape_sequence_parser(std::shared_ptr<character_iterator> iterator);

};


#endif