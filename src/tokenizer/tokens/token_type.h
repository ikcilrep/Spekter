#ifndef token_type_h
#define token_type_h

namespace spekter
{
    enum class token_type
    {
        CURLY_BRACKET_OPEN,
        CURLY_BRACKET_CLOSE,
        SQUARE_BRACKET_OPEN,
        SQUARE_BRACKET_CLOSE,
        PARENTHESIS_OPEN,
        PARENTHESIS_CLOSE,

        ARROW_OPERATOR,
        PLUS_OPERATOR,
        MINUS_OPERATOR,
        TIMES_OPERATOR,
        DIVIDE_OPERATOR,
        POWER_OPERATOR,
        AND_OPERATOR,
        OR_OPERATOR,
        XOR_OPERATOR,
        NEGATION_OPERATOR,
        DOT_OPERATOR,


        IF_KEYWORD,
        UNLESS_KEYWORD,
        WHILE_KEYWORD,
        FOR_KEYWORD,
        LOOP_KEYWORD,
        LET_KEYWORD,
        CLASS_KEYWORD,
        INTERFACE_KEYWORD,
        ABSTRACT_KEYWORD,
        OVERRIDE_KEYWORD,
        BOOLEAN_KEYWORD,
        STRING_KEYWORD,
        CHAR_KEYWORD,
        INT8_KEYWORD,
        INT16_KEYWORD,
        INT32_KEYWORD,
        INT64_KEYWORD,
        UINT8_KEYWORD,
        UINT16_KEYWORD,
        UINT32_KEYWORD,
        UINT64_KEYWORD,
        FLOAT32_KEYWORD,
        FLOAT64_KEYWORD,
        VOID_KEYWORD,
        PUBLIC_KEYWORD,
        PRIVATE_KEYWORD,
        PROTECTED_KEYWORD,
        

        STRING_LITERAL,
        INT_LITERAL,
        FLOAT_LITERAL,
        BOOL_LITERAL,
        CHAR_LITERAL,

        IDENTIFIER
    };
}

#endif