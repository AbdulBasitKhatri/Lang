#include "Parser.h"
#ifndef EVALUATOR_H_INCLUDED
#define EVALUATOR_H_INCLUDED

class Evaluator{
    std::vector<Node*> nodes;
    int cni = 0;
    Node *at(int i=0);
    void rem();
    float ConvNum(Token n);
    Token Add(Token l,Token r);
    Token Sub(Token l,Token r);
    Token Mul(Token l,Token r);
    Token Div(Token l,Token r);
    Token Mod(Token l,Token r);
    Token Pow(Token l,Token r);
    Token EvBinOp(Node *n);
    Token EvWhile(Node *n);
    Token EvFor(Node *n);
    Token EvIf(Node *n);
    Token EvComp(Node *n);
    Token EvLogical(Node *n);
    Token GetIden(Context *c,Token t);
    Token SetIden(Context *c,Token t,Token v,bool sameCont);
public:
    Evaluator();
    Evaluator(std::vector<Node*> _nodes);
    Token autoNode(Node *n);
};


#endif // EVALUATOR_H_INCLUDED
