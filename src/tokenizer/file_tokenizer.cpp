#include "file_tokenizer.h" 
#include <fstream>

using namespace spekter;

file_tokenizer::file_tokenizer(const std::string& code_path) :
    tokenizer(std::make_unique<std::ifstream>(code_path)){}