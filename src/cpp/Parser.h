#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
#include "ParseTree.h"

/*-----------------------------------------------------------------------------
Here is the parser. This is the star of the show. Give it a linear stream of
tokens and it will give you a colorfull (as in, interesting looking) syntax
tree. This parser is capable of parsing only arithmetic expressions. Uses the
following CFG to parse expressions:

Expression:
        Term
        Term "+" Expression
        Term "-" Expression
Term:
        Factor
        Factor "*" Term
        Factor "/" Term
Factor:
        floating-point-number
        "+" floating-point-number
        "-" floating-point-number
        "(" Expression ")"

-----------------------------------------------------------------------------*/
class Parser{
public:
    ParseNode parse(std::vector<Token>::iterator tokenIt);
private:
    ParseNode parseFactor(std::vector<Token>::iterator& tokenIt);
    ParseNode parseTerm(std::vector<Token>::iterator& tokenIt);
    ParseNode parseExpression(std::vector<Token>::iterator& tokenIt);
};
#endif