#include "Evaluator.h"

Evaluator::Evaluator(std::vector<Node*> _nodes):nodes(_nodes){
    while(cni < nodes.size()){
        Token t = autoNode(at());
        //std::cout << "{ type : " << t.type << ", value : " << t.val << " }" << std::endl;
        cni++;
    }
}

Node* Evaluator::at(int i){
    return nodes[cni+i];
}
void Evaluator::rem(){
    nodes.erase(nodes.begin());
}
float Evaluator::ConvNum(Token n){
    if(n.type == TT_INT){
        return (int) stoi(n.val);
    }
    else{
        return stof(n.val);
    }
}
Token Evaluator::Add(Token l,Token r){
    //IF ANY OF THEM IS STRING/BOOL THEN RETURN CONCAT STRING
    if((l.type == TT_STRING || r.type == TT_STRING) || (l.type == TT_BOOL || r.type == TT_BOOL) || (l.type == TT_NULL || r.type == TT_NULL)){return CreateToken(TT_STRING,l.val+r.val,l.ln);}
    else if(l.type == TT_FLOAT || r.type == TT_FLOAT){
        return CreateToken(TT_FLOAT,std::to_string(stof(l.val)+stof(r.val)),l.ln);
    }
    return CreateToken(TT_INT,std::to_string(stoi(l.val)+stoi(r.val)),l.ln);
}
Token Evaluator::Sub(Token l,Token r){
    //IF ANY OF THEM IS STRING/BOOL THEN RETURN CONCAT STRING
    if((l.type == TT_STRING || r.type == TT_STRING) || (l.type == TT_BOOL || r.type == TT_BOOL) || (l.type == TT_NULL || r.type == TT_NULL)){return CreateToken(TT_STRING,l.val+r.val,l.ln);}
    else if(l.type == TT_FLOAT || r.type == TT_FLOAT){
        return CreateToken(TT_FLOAT,std::to_string(stof(l.val)-stof(r.val)),l.ln);
    }
    return CreateToken(TT_INT,std::to_string(stoi(l.val)-stoi(r.val)),l.ln);
}
Token Evaluator::Mul(Token l,Token r){
    if(l.type == TT_STRING && r.type == TT_INT){
        int ri = stoi(r.val);
        std::string lv = l.val;
        if(ri == 0){l.val = "";}
        else{
            for(int i=1;i<ri;i++){l.val+=lv;}
        }
        return CreateToken(TT_STRING,l.val,l.ln);
    }
    //IF ANY OF THEM IS STRING/BOOL THEN RETURN CONCAT STRING
    else if((l.type == TT_STRING || r.type == TT_STRING) || (l.type == TT_BOOL || r.type == TT_BOOL) || (l.type == TT_NULL || r.type == TT_NULL)){return CreateToken(TT_STRING,l.val+r.val,l.ln);}
    else if(l.type == TT_FLOAT || r.type == TT_FLOAT){
        return CreateToken(TT_FLOAT,std::to_string(stof(l.val)*stof(r.val)),l.ln);
    }
    return CreateToken(TT_INT,std::to_string(stoi(l.val)*stoi(r.val)),l.ln);
}
Token Evaluator::Div(Token l,Token r){
    //IF ANY OF THEM IS STRING/BOOL THEN RETURN CONCAT STRING
    if(r.type == TT_INT || r.type == TT_FLOAT){if(stoi(r.val) == 0){GeneralErr(l,"Dividing By Zero Is Not Allowed!");/*can't Div by zero!*/}}
    if((l.type == TT_STRING || r.type == TT_STRING) || (l.type == TT_BOOL || r.type == TT_BOOL) || (l.type == TT_NULL || r.type == TT_NULL)){return CreateToken(TT_STRING,l.val+r.val,l.ln);}
    else if(l.type == TT_FLOAT || r.type == TT_FLOAT){
        return CreateToken(TT_FLOAT,std::to_string(stof(l.val)/stof(r.val)),l.ln);
    }
    else if(stoi(l.val)%stoi(r.val) != 0){
        return CreateToken(TT_FLOAT,std::to_string(stof(l.val)/stof(r.val)),l.ln);
    }
    return CreateToken(TT_INT,std::to_string(stoi(l.val)/stoi(r.val)),l.ln);
}
Token Evaluator::Mod(Token l,Token r){
    //IF ANY OF THEM IS STRING/BOOL THEN RETURN CONCAT STRING
    if(r.type == TT_INT || r.type == TT_FLOAT){if(stoi(r.val) == 0){GeneralErr(l,"Modulo By Zero Is Not Allowed!");/*can't Div by zero!*/}}
    if((l.type == TT_STRING || r.type == TT_STRING) || (l.type == TT_BOOL || r.type == TT_BOOL) || (l.type == TT_NULL || r.type == TT_NULL)){return CreateToken(TT_STRING,l.val+r.val,l.ln);}
    else if(l.type == TT_FLOAT || r.type == TT_FLOAT){
        return CreateToken(TT_FLOAT,std::to_string(fmod(stof(l.val),stof(r.val))),l.ln);
    }
    else if(stoi(l.val)%stoi(r.val) != 0){
        return CreateToken(TT_FLOAT,std::to_string(fmod(stof(l.val),stof(r.val))),l.ln);
    }
    return CreateToken(TT_INT,std::to_string(stoi(l.val)%stoi(r.val)),l.ln);
}
Token Evaluator::Pow(Token l,Token r){
    //IF ANY OF THEM IS STRING/BOOL THEN RETURN CONCAT STRING
    if((l.type == TT_STRING || r.type == TT_STRING) || (l.type == TT_BOOL || r.type == TT_BOOL) || (l.type == TT_NULL || r.type == TT_NULL)){return CreateToken(TT_STRING,l.val+r.val,l.ln);}
    else if(l.type == TT_FLOAT || r.type == TT_FLOAT){
        return CreateToken(TT_FLOAT,std::to_string(pow(stof(l.val),stof(r.val))),l.ln);
    }
    return CreateToken(TT_INT,std::to_string((int) pow(stoi(l.val),stoi(r.val))),l.ln);
}
Token Evaluator::GetIden(Context *c,Token t){
    if(c->vars.find(t.val) != c->vars.end()){
        return c->vars[t.val];
    }
    else if(c->parent){
        return GetIden(c->parent,t);
    }
    return CreateToken(TT_EMPTY,"empty",t.ln);
}
Token Evaluator::SetIden(Context *c,Token t,Token v,bool sameCont = false){
    if(sameCont){c->vars[t.val] = v;}
    if(c->vars.find(t.val) != c->vars.end()){
        c->vars[t.val] = v;
        return c->vars[t.val];
    }
    else if(c->parent){
        return SetIden(c->parent,t,v);
    }
    return CreateToken(TT_EMPTY,"empty",t.ln);
}
Token Evaluator::autoNode(Node *n){
    if(n->type == "BinOp"){return EvBinOp(n);}
    else if(n->type == "Assign"){
        //n->context->vars[n->token.val] = autoNode(n->left);
        Token left = autoNode(n->left);
        Token val = SetIden(n->context,n->token,left);
        if(val.type != TT_EMPTY){
            //std::cout<<cCont->name<<std::endl;
            return val;
        }
        else{
            //std::cout<<n->context->name<<std::endl;
            n->context->vars[n->token.val] = left;

            return left;
        }
    }
    else if(n->type == "Iden"){
        //std::cout<<cCont->name<<std::endl;
        Token val = GetIden(n->context,n->token);
        if(val.type != TT_EMPTY){
            return val;
        }
        else{GeneralErr(n->token,"Variable Is Not Defined In This Context!");}
    }
    else if(n->type == "Func"){
        if(n->token.val == "print"){
            std::cout << autoNode(n->left).val<<std::endl;
            return CreateToken(TT_BOOL,"true",n->token.ln);
        }
    }
    else if(n->type == "Unary"){
        if(n->token.type == TT_MINUS){
            Token t = autoNode(n->left);
            if(t.type == TT_INT){t.val = std::to_string(stoi(t.val) * -1);}
            else if(t.type == TT_FLOAT){t.val = std::to_string(stof(t.val) * -1);}
            return t;
        }
        if(n->token.type == TT_NOT){
            Token t = autoNode(n->left);
            if(t.type == TT_INT || t.type == TT_FLOAT){
                t.type = TT_BOOL;t.val = "false";
                if(ConvNum(t) == 0){t.type = TT_BOOL;t.val = "true";}
            }
            else if(t.type == TT_BOOL){
                if(t.val == "true"){t.val = "false";}
                else if(t.val == "false"){t.val = "true";}
            }
            else{
                t.type = TT_BOOL;t.val = "true";
            }
            return t;
        }
        return CreateToken(TT_NULL,"null",0);
    }
    else if(n->type == "Block"){
        if(n->nodes.size() > 0){
            for(Node *_n:n->nodes){
                Token t = autoNode(_n);
                if(t.type == TT_SKIP){
                    break;
                }
                else if(t.type == TT_BREAK){
                    return t;
                }
                else if(t.type == TT_KEYWORD){
                    if(t.val == "skip"){
                        return CreateToken(TT_SKIP,"skip",t.ln);
                    }
                    else if(t.val == "break"){
                        return CreateToken(TT_BREAK,"break",t.ln);
                    }
                }
            }
        }
        return CreateToken(TT_NULL,"null",0);
    }
    else if(n->type == "For"){
        return EvFor(n);
    }else if(n->type == "While"){
        return EvWhile(n);
    }
    else if(n->type == "If"){
        return EvIf(n);
    }
    else if(n->type == "Comp"){
        return EvComp(n);
    }
    else if(n->type == "Logical"){
        return EvLogical(n);
    }
    else if(n->type == "Single"){
        //std::cout<<n->token.val<<std::endl;
        return n->token;
    }
    else if(n->type == "Val"){
        return n->token;
    }
    else{
        return CreateToken(TT_NULL,"null",0);
    }
}
Token Evaluator::EvBinOp(Node *n){
    if(n->left && n->right){
        TokenType op = n->token.type;
        Token left = autoNode(n->left);
        Token right = autoNode(n->right);
        if(op == TT_PLUS){
            return Add(left,right);
        }
        else if(op == TT_MINUS){
            return Sub(left,right);
        }
        else if(op == TT_MUL){
            return Mul(left,right);
        }
        else if(op == TT_DIV){
            return Div(left,right);
        }
        else if(op == TT_MOD){
            return Mod(left,right);
        }
        else if(op == TT_POW){
            return Pow(left,right);
        }
    }
    return CreateToken(TT_NULL,"null",0);
}
Token Evaluator::EvFor(Node *n){
    if(n->left && n->right && n->other && n->stater){
        cCont = n->context;
        Token iter = autoNode(n->left);
        Token limit = autoNode(n->right);
        Token step = autoNode(n->other);
        iter.type = step.type;
        for(Node *_n:n->nodes){
            SetIden(cCont,_n->token,iter,true);
        }
        float _iter = ConvNum(iter);
        float _limit = ConvNum(limit);
        float _step = ConvNum(step);
        if(_iter <= _limit && _step > 0){
            for(_iter;_iter<=_limit;_iter+=_step){
                if(iter.type == TT_INT){
                    iter.val = std::to_string((int) _iter);
                }
                else{
                    iter.val = std::to_string(_iter);
                }
                SetIden(cCont,n->nodes[0]->token,iter);
                Token retval = autoNode(n->stater);
                if(retval.type == TT_BREAK){
                    break;
                }
            }
        }
        else if(_iter >= _limit && _step < 0){
            for(_iter;_iter>=_limit;_iter+=_step){
                if(iter.type == TT_INT){
                    iter.val = std::to_string((int) _iter);
                }
                else{
                    iter.val = std::to_string(_iter);
                }
                SetIden(cCont,n->nodes[0]->token,iter);
                Token retval = autoNode(n->stater);
                if(retval.type == TT_BREAK){
                    break;
                }
            }
        }
        cCont = cCont->parent;
    }
    return CreateToken(TT_NULL,"null",0);
}
Token Evaluator::EvWhile(Node *n){
    if(n->left && n->right){
        Token cond = autoNode(n->left);
        while(cond.val == "true"){
            Token retval = autoNode(n->right);
            if(retval.type == TT_KEYWORD){
                if(retval.val == "skip"){continue;}
                else if(retval.val == "break"){break;}
            }
            else if(retval.type == TT_BREAK){
                break;
            }
            else if(retval.type == TT_SKIP){
                continue;
            }
            cond = autoNode(n->left);
        }
    }
}
Token Evaluator::EvIf(Node *n){
    if(n->left && n->right){
        Token cond = autoNode(n->left);
        if(cond.val == "true"){
            Token t = autoNode(n->right);
            if(t.type == TT_SKIP || (t.type == TT_KEYWORD && t.val == "skip")){
                return CreateToken(TT_SKIP,"skip",t.ln);
            }
            else if(t.type == TT_BREAK || (t.type == TT_KEYWORD && t.val == "break")){
                return CreateToken(TT_BREAK,"break",t.ln);
            }
        }else if(n->other){
            autoNode(n->other);
        }
    }
    return CreateToken(TT_NULL,"null",0);
}
Token Evaluator::EvComp(Node *n){
    if(n->left && n->right){
        TokenType op = n->token.type;
        Token left = autoNode(n->left);
        Token right = autoNode(n->right);
        if(op == TT_EE){
            //STRING TO STRING
            if(left.type == TT_STRING && right.type == TT_STRING){
                if(left.val == right.val){return CreateToken(TT_BOOL,"true",left.ln);}
                else{return CreateToken(TT_BOOL,"false",left.ln);}
            }
            //NUMBER TO NUMBER
            else if((left.type == TT_INT || left.type == TT_FLOAT) && (right.type == TT_INT || right.type == TT_FLOAT)){
                float ln = ConvNum(left);
                float rn = ConvNum(right);
                if(ln == rn){return CreateToken(TT_BOOL,"true",left.ln);}
                else{return CreateToken(TT_BOOL,"false",left.ln);}
            }
            //NULL TO NULL
            else if(left.type == TT_NULL && right.type == TT_NULL){
                return CreateToken(TT_BOOL,"true",left.ln);
            }
            //BOOL TO BOOL
            else if(left.type == TT_BOOL && right.type == TT_BOOL){
                if(left.val == right.val){return CreateToken(TT_BOOL,"true",left.ln);}
                else{return CreateToken(TT_BOOL,"false",left.ln);}
            }
            else {
                return CreateToken(TT_BOOL,"false",left.ln);
            }
        }
        else if(op == TT_NE){
            //STRING TO STRING
            if(left.type == TT_STRING && right.type == TT_STRING){
                if(left.val != right.val){return CreateToken(TT_BOOL,"true",left.ln);}
                else{return CreateToken(TT_BOOL,"false",left.ln);}
            }
            //NUMBER TO NUMBER
            else if((left.type == TT_INT || left.type == TT_FLOAT) && (right.type == TT_INT || right.type == TT_FLOAT)){
                float ln = ConvNum(left);
                float rn = ConvNum(right);
                if(ln != rn){return CreateToken(TT_BOOL,"true",left.ln);}
                else{return CreateToken(TT_BOOL,"false",left.ln);}
            }
            //NULL TO NULL
            else if(left.type == TT_NULL && right.type == TT_NULL){
                return CreateToken(TT_BOOL,"false",left.ln);
            }
            //BOOL TO BOOL
            else if(left.type == TT_BOOL && right.type == TT_BOOL){
                if(left.val != right.val){return CreateToken(TT_BOOL,"true",left.ln);}
                else{return CreateToken(TT_BOOL,"false",left.ln);}
            }
            else {
                return CreateToken(TT_BOOL,"true",left.ln);
            }
        }
        else if(op == TT_GT){
            //NUMBER TO NUMBER
            if((left.type == TT_INT || left.type == TT_FLOAT) && (right.type == TT_INT || right.type == TT_FLOAT)){
                float ln = ConvNum(left);
                float rn = ConvNum(right);
                if(ln > rn){return CreateToken(TT_BOOL,"true",left.ln);}
                else{return CreateToken(TT_BOOL,"false",left.ln);}
            }
            return CreateToken(TT_BOOL,"false",left.ln);
        }
        else if(op == TT_LT){
            //NUMBER TO NUMBER
            if((left.type == TT_INT || left.type == TT_FLOAT) && (right.type == TT_INT || right.type == TT_FLOAT)){
                float ln = ConvNum(left);
                float rn = ConvNum(right);
                if(ln < rn){return CreateToken(TT_BOOL,"true",left.ln);}
                else{return CreateToken(TT_BOOL,"false",left.ln);}
            }
            return CreateToken(TT_BOOL,"false",left.ln);
        }
        else if(op == TT_GTE){
            //NUMBER TO NUMBER
            if((left.type == TT_INT || left.type == TT_FLOAT) && (right.type == TT_INT || right.type == TT_FLOAT)){
                float ln = ConvNum(left);
                float rn = ConvNum(right);
                if(ln >= rn){return CreateToken(TT_BOOL,"true",left.ln);}
                else{return CreateToken(TT_BOOL,"false",left.ln);}
            }
            else if(left.type == TT_NULL && right.type == TT_NULL){
                return CreateToken(TT_BOOL,"true",left.ln);
            }
            //BOOL TO BOOL
            else if(left.type == TT_BOOL && right.type == TT_BOOL){
                if(left.val == right.val){return CreateToken(TT_BOOL,"true",left.ln);}
                else{return CreateToken(TT_BOOL,"false",left.ln);}
            }
            return CreateToken(TT_BOOL,"false",left.ln);
        }
        else if(op == TT_LTE){
            //NUMBER TO NUMBER
            if((left.type == TT_INT || left.type == TT_FLOAT) && (right.type == TT_INT || right.type == TT_FLOAT)){
                float ln = ConvNum(left);
                float rn = ConvNum(right);
                if(ln <= rn){return CreateToken(TT_BOOL,"true",left.ln);}
                else{return CreateToken(TT_BOOL,"false",left.ln);}
            }
            else if(left.type == TT_NULL && right.type == TT_NULL){
                return CreateToken(TT_BOOL,"true",left.ln);
            }
            //BOOL TO BOOL
            else if(left.type == TT_BOOL && right.type == TT_BOOL){
                if(left.val == right.val){return CreateToken(TT_BOOL,"true",left.ln);}
                else{return CreateToken(TT_BOOL,"false",left.ln);}
            }
            return CreateToken(TT_BOOL,"false",left.ln);
        }
    }
    return CreateToken(TT_NULL,"null",0);
}
Token Evaluator::EvLogical(Node *n){
    Token left = autoNode(n->left);
    Token right = autoNode(n->right);
    if(n->token.val == "and"){
        if(left.val == "true" && right.val == "true"){return CreateToken(TT_BOOL,"true",left.ln);}
        return CreateToken(TT_BOOL,"false",left.ln);
    }
    else if(n->token.val == "or"){
        if(left.val == "true" || right.val == "true"){return CreateToken(TT_BOOL,"true",left.ln);}
        return CreateToken(TT_BOOL,"false",left.ln);
    }
}
