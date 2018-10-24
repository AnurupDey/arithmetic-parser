#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>

enum parseState{
    PSNONE,
    PSNUMBER,
    PSOPPERATION
};

std::vector<std::string> tokenize(std::string input){
    std::vector<std::string> tokens;
    parseState state = PSNONE;

    if(input.length() == 0 ) return tokens;

    unsigned int i = 0;
    
    std::string curToken = "";
    
    while(i <= input.length()){
        char curChar = input[i];
        switch(state){
            case PSNONE:{
                if(isdigit(curChar)){
                    state = PSNUMBER;
                    if(curToken != ""){
                        tokens.push_back(curToken);
                        curToken = "";
                    }
                } else if (curChar == '+' || curChar == '-' || curChar == '*' || curChar == '/'){
                    state = PSOPPERATION;
                    if(curToken != ""){
                        tokens.push_back(curToken);
                        curToken = "";
                    }
                }else i++;
            } break;
            case PSNUMBER: {
                if(isdigit(curChar)){
                    curToken += curChar;
                    i++;
                } else {
                    state = PSNONE;
                    tokens.push_back(curToken);
                    curToken = "";
                } 
            } break;
            case PSOPPERATION:{
                if (curChar == '+' || curChar == '-' || curChar == '*' || curChar == '/'){
                    curToken += curChar;
                    i++;
                } else {
                    state = PSNONE;
                    tokens.push_back(curToken);
                    curToken = "";
                }
            } break;
        }
    }

    return tokens;
} 


int main(){
    std::ifstream in("../../data/input.txt");
    if(!in){
        std::cerr << "Error opening file\n";
        return 1;
    }

    char buffer[1024];
    in.getline(buffer,1024,';');
    std::string input = buffer;
    std::cout << "Buffer: " << buffer << std::endl;
    std::vector<std::string> tokens = tokenize(input);
    for(auto& token : tokens){
        std::cout << token << std::endl;
    }

    in.close();
    return 0;
}