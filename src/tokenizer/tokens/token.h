#ifndef tokens_h 
#define tokens_h

#include "token_type.h"
#include <string>
#include <optional>

namespace spekter {

class token {
    public:
        token_type type;
        int line_number;
        int char_in_line_number; 
        int char_number;
        std::optional<std::string> text;
        
        token(token_type type, int line_number,int char_in_line_number, int char_number);
        token(token_type type, int line_number,int char_in_line_number, int char_number, std::string text); 
};
}

#endif
