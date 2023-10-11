#include <fstream>
#include "Error.h"

#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

class Lexer
{
    public:
        Lexer();
        Lexer(std::string cPath);
        std::vector<Token> GetTokens();

    private:
        void ReadCode(std::string cPath);
        std::vector<std::string> words;
        std::vector<Token> tokens;
        int lineno;
        int chindex;
        bool IsChar(char c);
        bool IsIden(char c);
        bool IsNum(char c);
        bool IsWhite(char c);
        bool IsOp(TokenType tt);
        char at();
        bool Exists(std::string arr[],int s,std::string target);
        void Pusher(std::string t,TokenType tt);
        void Pusher(char t,TokenType tt);
        void MakeWord();
        void MakeNumber();
        void MakeString();
        void MakeTokens();
};

#endif // LEXER_H_INCLUDED
