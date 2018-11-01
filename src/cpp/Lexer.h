#ifndef _LEXER_H_
#define _LEXER_H_

#include <vector>
#include <string>
#include "Token.h"
/*-----------------------------------------------------------------------------
This File contains the Lexer. Its just one function, so it is not encapsulated
into a class yet. See implementation for details
-----------------------------------------------------------------------------*/
std::vector<Token> tokenize(std::string input);

#endif