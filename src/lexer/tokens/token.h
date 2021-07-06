#ifndef tokens_h 
#define tokens_h

#include "token_type.h"
#include <string>

namespace spekter {

class token {
    std::string text;
    public:
        token_type type;
        int line_number;
        int char_in_line_number; 
        int char_number;
        std::string get_text();
};
}

#endif
