#include "tokenizer.h"
#include "tokens/token.h"

using namespace spekter;

tokenizer::tokenizer(std::unique_ptr<std::istream> code)
{
    this->code = std::move(code);
}
