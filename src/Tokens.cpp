#include "Tokens.h"

std::string KeyWords[11] = {"if","elif","else","print","and","or","while","for","skip","break","func"};

Token CreateToken(TokenType tt,std::string v,int l,int col){
    Token t;
    t.type = tt;
    t.val = v;
    t.ln = l;
    t.col = col;
    return t;
}
Token CreateToken(TokenType tt,char v,int l,int col){
    Token t;
    t.type = tt;
    t.val = std::string(1,v);
    t.ln = l;
    t.col = col;
    return t;
}
