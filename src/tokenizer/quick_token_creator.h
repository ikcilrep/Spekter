#ifndef quick_token_creator_h
#define quick_token_creator_h

#include <memory>
#include "character_iterator.h"
#include "tokens/token.h"

namespace spekter {
    class quick_token_creator {
        std::shared_ptr<character_iterator> iterator;

    public:
        token create_token(token_type type);
        token create_token(token_type type, std::string text);

        quick_token_creator();
        quick_token_creator(std::shared_ptr<character_iterator> iterator);

    };
}

#endif