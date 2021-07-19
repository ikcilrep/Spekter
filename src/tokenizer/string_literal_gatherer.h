#ifndef string_literal_gatherer_h
#define string_literal_gatherer_h

#include <memory>
#include "character_iterator.h"
#include "escape_sequence_parser.h"

namespace spekter {
    class string_literal_gatherer {
        std::shared_ptr<character_iterator> iterator;
        escape_sequence_parser escape_parser;
    public:
        std::string gather_characters();
        string_literal_gatherer();
        string_literal_gatherer(std::shared_ptr<character_iterator> iterator);
    };
}
#endif 
