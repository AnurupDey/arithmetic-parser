#include "Parser.h"

#include <iostream>
#define CRASH() {std::cout << "Controlled Crash\n"; int* ptr = nullptr; *ptr = 0;};


ParseNode Parser::parseTerm(std::vector<Token>::iterator& tokenIt){
    ParseNode node;
    node.name = "Term";
    node.subNodes.push_back(parsePrimary(tokenIt));
    if(tokenIt->type != TTEND){
        if(tokenIt->type==TTBRACKET && tokenIt->value == ")"){
            tokenIt++;
        } else if(tokenIt->type == TTOPERATOR){
            if(tokenIt->value == "*" || tokenIt->value == "/"){
                node.tokens.push_back(*tokenIt);
                tokenIt++;
                node.subNodes.push_back(parseTerm(tokenIt));
            } else CRASH();
        } else CRASH();
    }
    return node;
}

ParseNode Parser::parseExpression(std::vector<Token>::iterator& tokenIt){
    ParseNode node;
    node.name = "Expression";
    node.subNodes.push_back(parseTerm(tokenIt));
    if(tokenIt->type != TTEND && (tokenIt->type!=TTBRACKET && tokenIt->value != ")")){
        if(tokenIt->type == TTOPERATOR){
            if(tokenIt->value == "+" || tokenIt->value == "-"){
                node.tokens.push_back(*tokenIt);
                tokenIt++;
                node.subNodes.push_back(parseExpression(tokenIt));
            } else CRASH();
        } else CRASH();
    }
    return node;
}

ParseNode Parser::parsePrimary(std::vector<Token>::iterator& tokenIt){
    ParseNode node;
    node.name = "Primary";
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
        tokenIt++; //skip the closing bracket.
    } else CRASH();
    return node;
}

ParseNode Parser::parse(std::vector<Token>::iterator tokenIt){
    ParseNode curNode = parseExpression(tokenIt);
    return curNode;
}
