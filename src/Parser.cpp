#include "Parser.h"
//std::map<std::string,Token> VARS = {};
Context *cCont = nullptr;
std::vector<Context*> contexts = {};

Parser::Parser(std::vector<Token> t,std::string cPath){
    tokens = t;
    ast = {};
    conti = 0;
    cCont = new Context(cPath,true,nullptr);
    conti++;
    contexts.push_back(cCont);
    while(tokens.size() != 0){
        GenerateAst();
    }
    /*for(Node *n:ast){printNode(*n);std::cout << std::endl;}*/
}
std::vector<Node*> Parser::GetAST(){
    return ast;
}
Token Parser::eat(){
    Token t = *tokens.begin();
    tokens.erase(tokens.begin());
    return t;
}
std::string Parser::TokenTypeRes(TokenType t){
    switch(t){
        case TT_IDENTIFIER:
            return "Identifier";
        case TT_STRING:
            return "String";
        case TT_FLOAT:
            return "Float";
        case TT_INT:
            return "Int";
        case TT_KEYWORD:
            return "Keyword";
        case TT_LPAREN:
            return "(";
        case TT_RPAREN:
            return ")";
        case TT_LCURLY:
            return "{";
        case TT_RCURLY:
            return "}";
        case TT_LBOX:
            return "[";
        case TT_RBOX:
            return "]";
        case TT_COLON:
            return ":";
    }
}
Token Parser::ExpectToken(TokenType t,std::string err){
    Token _t = eat();
    if(_t.type != t){GeneralErr(_t,"Expected ' "+TokenTypeRes(t)+" ' "+err);}
    return _t;
}
Token Parser::at(int i){
    if(i>=tokens.size()){return CreateToken(TT_NULL,"null",0);}
    return tokens[i];
}
void Parser::printNode(Node n){
    std::cout << "{ " << " Type : " << n.type;
    std::cout << ", Name : " << n.token.val;
    if(n.left){
        std::cout << ", LN : ";
        printNode(*n.left);
    }
    if(n.right){
        std::cout <<  ", RN : ";
        printNode(*n.right);
    }
    if(n.other){
        std::cout <<  ", OT : ";
        printNode(*n.other);
    }
    if(n.stater){
        std::cout <<  ", ST : ";
        printNode(*n.stater);
    }
    if(n.nodes.size() > 0){
        std::cout << ", CH : [ ";
        for(Node* _n:n.nodes){
            printNode(*_n);
            std::cout << " , ";
        }
        std::cout << " ]";
    }
    if(n.context){
    std::cout << ", context : "<<n.context->name;
    }
    std::cout << " }";
}
/*Node* Parser::FuncExpr(){
    Token t =eat();
}*/
Node* Parser::ForExpr(){
    Token t = eat();
    //CHECK FOR OPEN PAREN
    ExpectToken(TT_LPAREN,"After 'for' Keyword!");
    //MAKE CONTEXT FOR 'FOR' LOOP
    Context *cont = new Context(t.val+std::to_string(conti),false,cCont);
    conti++;
    cCont = cont;
    //GET ITERATOR NAME
    Token iteratorToken = ExpectToken(TT_IDENTIFIER,"Expected Initializer Type Of 'Identifier'!");
    Node *iteratorNode;
    //CHECK FOR VALUE TO INITIALIZER
    if(at().type == TT_EQUAL){
        eat();
        iteratorNode = ArithExpr();
    }
    else{
        iteratorNode = new Node("Val",CreateToken(TT_INT,"0",t.ln));
    }
    if(iteratorNode->token.type == TT_EMPTY){GeneralErr(iteratorToken,"Identifier Must Be Initialized First!");}
    //GET LIMIT
    ExpectToken(TT_COLON,"!");
    Node *expr = ArithExpr();
    if(expr->token.type == TT_EMPTY){GeneralErr(t,"Limit Must Be Defined!");}
    //GET STEP
    Node *expr2;
    if(at().type == TT_COLON){
        eat();
        expr2 = ArithExpr();
    }
    else{
        expr2 = new Node("Val",CreateToken(TT_INT,"1",t.ln));
    }
    if(expr2->token.type == TT_EMPTY){GeneralErr(t,"When ' : ' Is Given In The End , Step Must Be Defined!");}
    //CHECK FOR CLOSING PAREN
    ExpectToken(TT_RPAREN,"' ( ' Was Never Closed!");
    //CHECK FOR OPENING CURLY
    ExpectToken(TT_LCURLY,"After ' ) ' !");
    //GET STATEMENTS
    Node* stats = BlockNoCont();
    //CHECK FOR CLOSING CURLY
    ExpectToken(TT_RCURLY,"For Ending 'for' Loop!");
    cCont = cCont->parent;
    std::vector<Node*> varstoinit = {new Node("Assign",iteratorToken,iteratorNode,nullptr,cont)};
    return new Node("For",t,iteratorNode,expr,cont,expr2,stats,varstoinit);
}
Node* Parser::WhileExpr(){
    Token t = eat();
    //CHECK FOR OPEN PAREN
    ExpectToken(TT_LPAREN,"After 'while' Keyword!");
    //GET LOGICAL EXPR
    Node *cond = LogcExpr();
    //CHECK FOR CLOSING PAREN
    ExpectToken(TT_RPAREN,"After 'while' Condition!");
    //CHECK FOR OPENING CURLY
    ExpectToken(TT_LCURLY,"After ' ) ' !");
    //GET STATEMENTS
    Node* stats = Block(t);
    //CHECK FOR CLOSING CURLY
    ExpectToken(TT_RCURLY,"For Ending 'while' Loop!");

    return new Node("While",t,cond,stats,cCont);
}
Node* Parser::IfExpr(bool isElif){
    Token t = eat();
    if(isElif){
    //CHECK FOR OPEN PAREN
        ExpectToken(TT_LPAREN,"After 'elif' Keyword!");
    }
    else{
    //CHECK FOR OPEN PAREN
        ExpectToken(TT_LPAREN,"After 'if' Keyword!");
    }
    //GET LOGICAL EXPR
    Node *cond = LogcExpr();
    if(isElif){
        //CHECK FOR CLOSING PAREN
        ExpectToken(TT_RPAREN,"After 'elif' Condition!");
    }
    else{
        //CHECK FOR CLOSING PAREN
        ExpectToken(TT_RPAREN,"After 'if' Condition!");
    }
    //CHECK FOR OPENING CURLY
    ExpectToken(TT_LCURLY,"After ' ) ' !");
    //GET STATEMENTS
    Node* stats = Block(t);
    if(isElif){
        //CHECK FOR CLOSING CURLY
        ExpectToken(TT_RCURLY,"For Ending 'elif' Statement!");
    }
    else{
        //CHECK FOR CLOSING CURLY
        ExpectToken(TT_RCURLY,"For Ending 'if' Statement!");
    }
    //CHECK FOR ELIF
    if(at().type == TT_KEYWORD && at().val == "elif"){
        //RETURN IF WITH RECURSIVE IF (ELIF)
        return new Node("If",t,cond,stats,cCont,IfExpr(true));
    }
    //CHECK FOR ELSE
    else if(at().type == TT_KEYWORD && at().val == "else"){
        eat();
        //CHECK FOR OPENING CURLY
        ExpectToken(TT_LCURLY,"After 'else' Statement!");
        //GET STATMENETS
        Node *elsestats = Block(t);
        //CHECK FOR CLOSING CURLY
        ExpectToken(TT_RCURLY,"For Ending 'else' statement!");
        //RETURN IF WITH ELSE
        return new Node("If",t,cond,stats,cCont,elsestats);
    }
    //RETURN JUST 'IF' IF NO ELIF/ELSE
    return new Node("If",t,cond,stats,cCont);
}
Node* Parser::Block(Token t){
    std::vector<Node*> stats = {};
    Context *cont = new Context(t.val+std::to_string(conti),false,cCont);
    conti++;
    cCont = cont;
    while (at().type != TT_RCURLY){
        Node *stat = Statement();
        if(stat->type != "Empty"){
            stats.push_back(stat);
        }
    }
    cCont = cCont->parent;
    if(stats.size() > 0){
    return new Node("Block",CreateToken(TT_BLOCK,"block",stats[0]->token.ln),nullptr,nullptr,cont,nullptr,nullptr,stats);
    }
    else{return new Node("Block",CreateToken(TT_BLOCK,"block",at().ln),nullptr,nullptr,cont,nullptr,nullptr,stats);}
}Node* Parser::BlockNoCont(){
    std::vector<Node*> stats = {};
    while (at().type != TT_RCURLY){
        Node *stat = Statement();
        if(stat->type != "Empty"){
        stats.push_back(stat);
        }
    }
    if(stats.size() > 0){
    return new Node("Block",CreateToken(TT_BLOCK,"block",stats[0]->token.ln),nullptr,nullptr,nullptr,nullptr,nullptr,stats);
    }
    else{return new Node("Block",CreateToken(TT_BLOCK,"block",at().ln),nullptr,nullptr,nullptr,nullptr,nullptr,stats);}
}
Node* Parser::Statement(){
    //It will check for keywords and other statements which makes expression
    if(at().type == TT_KEYWORD){
        //Print statement
        if(at().val == "print"){
            //Then here should be the expr closed under parenthesis
            Token func = eat();
            Token _t = eat();
            if(_t.type == TT_LPAREN){
                Node *n = LogcExpr();
                if(!n){exit(1);/*Missing Expression*/}
                    _t = eat();
                    if(_t.type == TT_RPAREN){
                        return new Node("Func",func,n,nullptr);
                    }
                else{exit(1);/*Missing Parenthesis during print statement*/}
            }else{exit(1);/*'print' is a function*/}
        }

    }
    //KEYWORDS
    if(at().type == TT_KEYWORD){
        //IF STATEMENT
        if(at().val == "if"){
            return IfExpr();
        }
        else if(at().val == "elif"){
            GeneralErr(at(),"'elif' Without 'if' Statement!");
        }
        else if(at().val == "else"){
            GeneralErr(at(),"'else' Without 'if' Statement!");
        }
        else if(at().val == "while"){
            return WhileExpr();
        }
        else if(at().val == "for"){
            return ForExpr();
        }
        else if(at().val == "skip" || at().val == "break"){
            return new Node("Single",eat(),nullptr,nullptr,cCont);
        }
    }
    //  VARIABLE ASSIGNMENT
    if(at().type == TT_IDENTIFIER){
        if(at(1).type == TT_EQUAL){
            Token t = eat();
            eat();
            return new Node("Assign",t,Statement(),nullptr,cCont);
        }
    }
    return LogcExpr();
}
Node* Parser::LogcExpr(){
    Node *left = CompExpr();
    if(at().type == TT_KEYWORD){
        if(at().val == "and" || at().val == "or"){
            Token ct = eat();
            left = new Node("Logical",ct,left,LogcExpr());
        }
    }
    return left;
}
Node* Parser::CompExpr(){
    if(at().type == TT_NOT){
        Token ct = eat();
        return new Node("Unary",ct,CompExpr());
    }
    Node *left = ArithExpr();

    if(at().type == TT_EE || at().type == TT_GT || at().type == TT_LT || at().type == TT_GTE || at().type == TT_LTE || at().type == TT_NE){
        Token ct = eat();
        left = new Node("Comp",ct,left,CompExpr());
    }
    return left;
}
Node* Parser::ArithExpr(){
    Node *left = Term();
    while (at().type == TT_PLUS || at().type == TT_MINUS){
        Token ct = eat();
        left = new Node("BinOp",ct,left,Term());
    }
    return left;
}
Node* Parser::Term(){
    Node *left = Factor();

    while (at().type == TT_MUL || at().type == TT_DIV || at().type == TT_MOD || at().type == TT_POW){
        Token ct = eat();
        left = new Node("BinOp",ct,left,Factor());
    }
    return left;
}
Node* Parser::Factor(){
    Node *left = Atom();

    while (at().type == TT_POW){
        Token ct = eat();
        left = new Node("BinOp",ct,left,Atom());
    }
    return left;
}
Node* Parser::Atom(){
    Token t = at();
    ////////////////////////////////////////////////////////////////////////////////////////////
    //
    if(t.type == TT_IDENTIFIER){
        t = eat();
        //RETURN THE IDEN NODE
        return new Node("Iden",t,nullptr,nullptr,cCont);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    else if(t.type == TT_NUM || t.type == TT_INT || t.type == TT_FLOAT || t.type == TT_STRING || t.type == TT_BOOL || t.type == TT_NULL){
        t = eat();
        return new Node("Val",t);
    }
    //                          CHECK FOR PARENTHESIS
    else if(t.type == TT_LPAREN){
        t = eat();
        Node *expr = LogcExpr();
        Token _t = eat();
        if(_t.type != TT_RPAREN){GeneralErr(_t,"Expected Closing Parenthesis!");}
        return expr;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////
    //                          UNARY OPERATORS HERE
    //
    else if(t.type == TT_MINUS){
        t = eat();
        return new Node("Unary",t,Atom(),nullptr);
    }
    //If Plus then just skip it
    else if(t.type == TT_PLUS){
        t = eat();
        return Factor();
    }
    //                      RETURN EMPTY IF NOTHING
    else{
        return new Node("Empty",CreateToken(TT_EMPTY,"empty",t.ln));
    }
}
void Parser::GenerateAst(){
    Node *a = Statement();
    //printNode(*a);
    if(!a){std::cout << "err";exit(1);}
    if(a->type != "Empty")ast.push_back(a);
}
