#ifndef conditional_character_gatherer_h
#define conditional_character_gatherer_h

#include <string>
#include <functional>
#include "character_iterator.h"

namespace spekter {
    class conditional_character_gatherer {
        std::shared_ptr<character_iterator> iterator;

    public:
        std::string gather_characters(std::function<bool(char)> is_in_group,
            std::function<bool(std::string)> is_finished = [](auto _) {return false;});
        conditional_character_gatherer();
        conditional_character_gatherer(std::shared_ptr<character_iterator> iterator);
    };
}

#endif