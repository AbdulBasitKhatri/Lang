#include "Error.h"

void PrintPointer(std::string v){
    std::cout << "\t near: " << v << std::endl;
    std::cout << "\t       ";
    for(int i=0;i<v.length();i++){std::cout << "^";}
    std::cout << std::endl;
}

void NumConErr(Token t){
    std::cout << "Lang: Error : Number cannot contain more than 1 decimal point!" << std::endl;
    PrintPointer(t.val);
    std::cout << "\tAt line " << t.ln+1 << ":" << t.col+1 << std::endl;
    exit(1);
}
void StrConErr(Token t){
    std::cout << "Lang: Error : String must be closed with ' \" '" << std::endl;
    PrintPointer(t.val);
    std::cout << "\tAt line " << t.ln+1 << ":" << t.col+1 << std::endl;
    exit(1);
}

void GeneralErr(Token t,std::string err){
    std::cout << "Lang: Error : " << err << std::endl;
    PrintPointer(t.val);
    std::cout << "\tAt line " << t.ln+1 << ":" << t.col+1 << std::endl;
    exit(1);
}
