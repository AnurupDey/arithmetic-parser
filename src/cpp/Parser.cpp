#include "Parser.h"

#include <iostream>
#define CRASH() {std::cout << "Controlled Crash\n"; int* ptr = nullptr; *ptr = 0;};


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

ParseNode Parser::parse(std::vector<Token>::iterator tokenIt){
    ParseNode curNode = parseExpression(tokenIt);
    return curNode;
}
