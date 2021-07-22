#ifndef quick_token_creator_h
#define quick_token_creator_h

#include <memory>
#include <unordered_map>
#include "character_iterator.h"
#include "tokens/token.h"

namespace spekter {
    class quick_token_creator {
        std::shared_ptr<character_iterator> iterator;

        static const std::unordered_map<std::string, token_type> constant_text_to_token_type;
    public:
        token create_token(token_type type);
        token create_token(token_type type, std::string text);
        token get_token_with_constant_text(const std::string& text);

        static bool is_text_constant(const std::string& text);



        quick_token_creator();
        quick_token_creator(std::shared_ptr<character_iterator> iterator);

    };
}

#endif