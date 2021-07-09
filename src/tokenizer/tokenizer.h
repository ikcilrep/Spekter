#ifndef tokenizer_h
#define tokenizer_h

#include <memory>
#include <istream>
#include "tokens/token.h"



namespace spekter
{
    class tokenizer
    {
        std::unique_ptr<std::istream> code;

    public:
        tokenizer(std::unique_ptr<std::istream> code);
        token next_token();
    };
}

#endif
