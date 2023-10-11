#include "Lexer.h"
#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

//std::map<std::string,Token> extern VARS;
//Context
//-> symbol table
//   -> var name/tokens with context-name
//-> function table
//   -> function name/nodes with context-name
struct Node;
struct Context{
    std::string name;
    bool global;
    std::map<std::string,Token> vars = {};
    std::map<std::string,Node*> funcs = {};
    Context *parent;
    Context(std::string _name,bool _global,Context *_parent = nullptr):name(_name),global(_global),parent(_parent){}
};
struct Node{
    std::string type = "";
    Token token;
    Node *left;
    Node *right;
    Node *other;
    Node *stater;
    std::vector<Node*> nodes;
    Context* context;
    Node(std::string _type,Token _t,Node *_left = nullptr,Node *_right = nullptr,Context* _context = nullptr,Node *_other = nullptr,Node *_stater = nullptr,std::vector<Node*> _nodes = {}):type(_type),token(_t),left(_left),right(_right),other(_other),nodes(_nodes),context(_context),stater(_stater) {}
};


extern Context *cCont;

extern std::vector<Context*> contexts;
//make a variable which tracks current context
//make a queue of global to any context & empty the queue after returning to global context

//Global Context names as file name

//functions can make their own context

//every statement will use their parent and global context

//function can contain anything but anything cant contain function except global context

//calling any function will change current context and return to its caller context

class Parser{

public:
    Parser();
    Parser(std::vector<Token> t,std::string cPath);
    std::vector<Node*> GetAST();
private:
    std::vector<Token> tokens;
    std::vector<Node*> ast;
    int conti;
    Token eat();
    std::string TokenTypeRes(TokenType t);
    Token ExpectToken(TokenType t,std::string err = "");
    Token at(int i=0);
    void printNode(Node n);
    Node *BlockNoCont();
    Node *Block(Token t);
    Node *ForExpr();
    Node *WhileExpr();
    Node *IfExpr(bool isElif = false);
    Node *Statement();
    Node *LogcExpr();
    Node *CompExpr();
    Node *ArithExpr();
    Node *Term();
    Node *Factor();
    Node *Atom();
    void GenerateAst();
};

#endif // PARSER_H_INCLUDED
