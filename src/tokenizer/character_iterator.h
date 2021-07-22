#ifndef character_iterator_h
#define character_iterator_h

#include <optional>
#include <istream>
#include <memory>
#include "tokens/token.h"

namespace spekter {
    class character_iterator {

        void update_line_number(char character);
        void increment_line_number();
        void set_current_character(char character);
        std::unique_ptr<std::istream> character_stream;
    public:
        character_iterator();
        character_iterator(std::unique_ptr<std::istream> character_stream);
        int char_number = 0;
        int line_number = 0;
        int char_in_line_number = 0;
        std::optional<char> current_character;
        void next_character();
        void skip_white_spaces();
    };
}

#endif