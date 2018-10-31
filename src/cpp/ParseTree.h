#ifndef _PARSETREE_H_
#define _PARSETREE_H_

#include <vector>
#include <string>

#include "Token.h"

class ParseNode {
public:
//private:
    ParseNode* parent;
    std::string name;

    std::vector<ParseNode> subNodes;
    std::vector<Token> tokens;

    static ParseNode Default;
};

#endif