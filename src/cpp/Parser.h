#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
#include "ParseTree.h"

class Parser{
public:
    ParseNode parse(std::vector<Token>::iterator tokenIt);
private:
    ParseNode parsePrimary(std::vector<Token>::iterator& tokenIt);
    ParseNode parseTerm(std::vector<Token>::iterator& tokenIt);
    ParseNode parseExpression(std::vector<Token>::iterator& tokenIt);
};
#endif