#include "Parser.h"
#include <iostream>

//NOTE: Crashing in order to solve a problem is never a good solution
#define CRASH() {std::cout << "Controlled Crash\n"; int* ptr = nullptr; *ptr = 0;};

/*-----------------------------------------------------------------------------
Parses a Term according to the grammar rule
Term:
    Factor
    Factor "*" Term
    Factor "/" Term
PARAM:
[IN/OUT]    tokenIt -   Iterator to the token stream, set at the position in
                        the stream from which to start parsing. This iterator
                        will be updated to be at the end of the parsed Term 
                        when the function returns.
RETURNS:
    A node containing the subtree representing the parsed Term
-----------------------------------------------------------------------------*/
ParseNode Parser::parseTerm(std::vector<Token>::iterator& tokenIt){
    ParseNode node;
    node.name = "Term";
    node.subNodes.push_back(parseFactor(tokenIt));
    while(tokenIt->type != TTEND){
        if(tokenIt->type==TTBRACKET && tokenIt->value == ")"){
            break;
        } else if(tokenIt->type == TTOPERATOR){
            if(tokenIt->value == "*" || tokenIt->value == "/"){
                node.tokens.push_back(*tokenIt);
                tokenIt++;
                node.subNodes.push_back(parseFactor(tokenIt));
            } else break;
        } else CRASH();
    }
    return node;
}

/*-----------------------------------------------------------------------------
Parses an Expression according to the grammar rule
Expression:
    Term
    Term "*" Expression
    Term "/" Expression
PARAM:
[IN/OUT]    tokenIt -   Iterator to the token stream, set at the position in
                        the stream from which to start parsing. This iterator
                        will be updated to be at the end of the parsed 
                        Expression when the function returns.
RETURNS:
    A node containing the subtree representing the parsed Expression
-----------------------------------------------------------------------------*/
ParseNode Parser::parseExpression(std::vector<Token>::iterator& tokenIt){
    ParseNode node;
    node.name = "Expression";
    node.subNodes.push_back(parseTerm(tokenIt));
    while(tokenIt->type != TTEND && (tokenIt->type!=TTBRACKET && tokenIt->value != ")")){
        if(tokenIt->type == TTOPERATOR){
            if(tokenIt->value == "+" || tokenIt->value == "-"){
                node.tokens.push_back(*tokenIt);
                tokenIt++;
                node.subNodes.push_back(parseTerm(tokenIt));
            } else CRASH()
        } else CRASH();
    }
    return node;
}

/*-----------------------------------------------------------------------------
Parses a Factor according to the grammar rule
Factor:
    floating-point-number
    "+" floating-point-number
    "-" floating-point-number
    "(" Expression ")"
PARAM:
[IN/OUT]    tokenIt -   Iterator to the token stream, set at the position in
                        the stream from which to start parsing. This iterator
                        will be updated to be at the end of the parsed Factor 
                        when the function returns.
RETURNS:
    A node containing the subtree representing the parsed Factor
-----------------------------------------------------------------------------*/
ParseNode Parser::parseFactor(std::vector<Token>::iterator& tokenIt){
    ParseNode node;
    node.name = "Factor";
    if(tokenIt->type == TTNUMBER){
        node.tokens.push_back(*tokenIt);
        tokenIt++;
    } else if(tokenIt->type == TTOPERATOR){
        if(tokenIt->value == "+" || tokenIt->value == "-"){
            node.tokens.push_back(*tokenIt);
            tokenIt++;
            if(tokenIt->type == TTNUMBER){
                node.tokens.push_back(*tokenIt);
                tokenIt++;
            } else CRASH();
        }
    } else if(tokenIt->type == TTBRACKET && tokenIt->value == "("){
        tokenIt++;
        node.subNodes.push_back(parseExpression(tokenIt));
        if(!(tokenIt->type == TTBRACKET && tokenIt->value == ")"))
            CRASH(); //we have to expect this.
        tokenIt++; //skip the closing bracket.
    } else CRASH();
    return node;
}

/*-----------------------------------------------------------------------------
Parses expression
PARAM:
[IN/OUT]    tokenIt -   Iterator to the token stream, set at the position in
                        the stream from which to start parsing. This iterator
                        will be updated to be at the end of the parsed Expression 
                        when the function returns.
RETURNS:
    The root node of the tree representing the parsed Expression
-----------------------------------------------------------------------------*/
ParseNode Parser::parse(std::vector<Token>::iterator tokenIt){
    ParseNode curNode = parseExpression(tokenIt);
    return curNode;
}
