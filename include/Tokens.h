#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>

#ifndef TOKENS_H_INCLUDED
#define TOKENS_H_INCLUDED

enum TokenType {
    TT_INT,
    TT_FLOAT,
    TT_NUM,
    TT_STRING,
    TT_KEYWORD,
    TT_IDENTIFIER,
    TT_EQUAL,
    TT_EE,
    TT_LPAREN,
    TT_RPAREN,
    TT_NOT,
    TT_NE,
    TT_SEMICOLON,
    TT_COLON,
    TT_LT,
    TT_LTE,
    TT_GT,
    TT_GTE,
    TT_LCURLY,
    TT_RCURLY,
    TT_LBOX,
    TT_RBOX,
    TT_PLUS,
    TT_MINUS,
    TT_MUL,
    TT_DIV,
    TT_MOD,
    TT_POW,
    TT_BOOL,
    TT_NULL,
    TT_BLOCK,
    TT_EMPTY,
    TT_SKIP,
    TT_BREAK
};

std::string extern KeyWords[11];

typedef struct{
    TokenType type;
    std::string val;
    int ln;
    int col;
} Token;

Token CreateToken(TokenType tt,std::string v,int l,int col=0);
Token CreateToken(TokenType tt,char v,int l,int col=0);

#endif // TOKENS_H_INCLUDED
