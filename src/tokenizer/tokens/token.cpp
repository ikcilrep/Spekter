#include "token.h"
#include <string> 

using namespace spekter;

token::token(token_type type, int line_number,int char_in_line_number, int char_number) :
    type(type),
    line_number(line_number),
    char_in_line_number(char_in_line_number),
    char_number(char_number) {}

token::token(token_type type, int line_number,int char_in_line_number, int char_number, std::string text) :
    type(type),
    line_number(line_number),
    char_in_line_number(char_in_line_number),
    char_number(char_number),
    text(std::make_optional<std::string>(text)) {}