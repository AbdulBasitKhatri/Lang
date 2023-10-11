#include "Tokens.h"

#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

void PrintPointer(std::string v);
void NumConErr(Token t);
void StrConErr(Token t);
void GeneralErr(Token t,std::string err);


#endif // ERROR_H_INCLUDED
