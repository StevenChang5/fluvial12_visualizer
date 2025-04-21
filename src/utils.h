#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>


inline std::vector<std::string> split_string(const std::string& str, char delimiter=' '){
    std::vector<std::string> tokens;
    std::string temp ="";
    for(int i = 0; i < str.size(); i++){
        char letter = str[i];
        if(letter == delimiter && temp != ""){
            tokens.push_back(temp);
            temp = "";
        }
        if(letter != delimiter){
            temp += letter;
        } 
    }
    if(temp != ""){
        tokens.push_back(temp);
    }
    return tokens;
}

#endif