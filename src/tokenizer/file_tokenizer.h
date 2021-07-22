#ifndef file_tokenizer_h
#define file_tokenizer_h

#include <string>
#include "tokenizer.h"

namespace spekter
{
    class file_tokenizer : tokenizer
    {
    public:
        file_tokenizer(const std::string& code_path);
    };
}

#endif