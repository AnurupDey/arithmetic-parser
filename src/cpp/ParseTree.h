#ifndef _PARSETREE_H_
#define _PARSETREE_H_

#include <vector>
#include <string>

#include "Token.h"

/*-----------------------------------------------------------------------------
Node of a Parse-Tree. This Tree structure is theoretically capable of 
representing any Parsed Tree Data, not just Arithmatic expressions.
-----------------------------------------------------------------------------*/
struct ParseNode {
    std::string name;

    std::vector<ParseNode> subNodes;
    std::vector<Token> tokens;

    static ParseNode Default;
};

#endif