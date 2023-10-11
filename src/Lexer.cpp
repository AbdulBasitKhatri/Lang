#include "Lexer.h"

Lexer::Lexer(std::string cPath)
{
    words = {};
    tokens = {};

    lineno = 0;
    chindex = 0;
    ReadCode(cPath);
}
void Lexer::ReadCode(std::string cPath){
    std::ifstream code(cPath);
    std::string word;
    if (code.is_open()) {
        while (code.good() && getline(code,word)) {
            words.push_back(word);
        }
    }
    MakeTokens();
}
bool Lexer::IsChar(char c){
        if((c >= 65 && c<= 90) || (c >= 97 && c <= 122)){return true;}
        return false;
    }
bool Lexer::IsIden(char c){
        if((c >= 65 && c<= 90) || (c >= 97 && c <= 122) || c == '_' || c == '$'){return true;}
        return false;
    }
bool Lexer::IsNum(char c){
        if(c >= 48 && c<= 57){return true;}
        return false;
    }
bool Lexer::IsWhite(char c){
        if(c == ' ' || c == '\n' || c == '\r' || c == '\t'){return true;}
        return false;
    }
char Lexer::at(){
        return words[lineno][chindex];
    }
bool Lexer::Exists(std::string arr[],int s,std::string target){
        for(int i=0;i<s;i++){
            if(arr[i]==target){
                return true;
            }
        }
        return false;
    }
bool Lexer::IsOp(TokenType tt){
        if(tt == TT_DIV || tt == TT_MUL || tt == TT_PLUS || tt == TT_MINUS || tt == TT_POW){return true;}
        return false;
    }
void Lexer::Pusher(std::string t,TokenType tt = TT_IDENTIFIER){
        if(t.length() > 0){
            if(Exists(KeyWords,sizeof(KeyWords)/sizeof(std::string),t)){tokens.push_back(CreateToken(TT_KEYWORD,t,lineno));}
            else if((t == "true" || t == "false") && tt == TT_IDENTIFIER){tokens.push_back(CreateToken(TT_BOOL,t,lineno));}
            else if(t == "null" && tt == TT_IDENTIFIER){tokens.push_back(CreateToken(TT_NULL,t,lineno));}
            else{tokens.push_back(CreateToken(tt,t,lineno,chindex));}
        }
        else if(tt == TT_STRING){tokens.push_back(CreateToken(tt,t,lineno,chindex));}
    }
void Lexer::Pusher(char t,TokenType tt = TT_IDENTIFIER){
        tokens.push_back(CreateToken(tt,t,lineno,chindex));
    }
void Lexer::MakeWord(){
        std::string s = "";
        while(chindex < words[lineno].length()){
            if(IsIden(at()) || IsNum(at())){s+=at();}
            else if(IsWhite(at())){Pusher(s);s="";break;}
            else{break;}
            chindex++;
        }
        Pusher(s);
    }
void Lexer::MakeNumber(){
        std::string s = "";
        int dotc = 0;
        while(chindex < words[lineno].length() && (IsNum(at()) || at() == '.')){
            if(at() == '.'){
                dotc++;
                if(dotc > 1){s+=at();NumConErr(CreateToken(TT_FLOAT,s,lineno));/*NUMBER CAN'T CONTAIN MORE THAN 1 DOT!*/}
            }
            s+=at();
            chindex++;
        }
        if(s.back() == '.'){s+="0";}
        if(dotc > 0){Pusher(s,TT_FLOAT);}
        else{Pusher(s,TT_INT);}
    }
void Lexer::MakeString(){
        std::string s = "";
        int quotec = 0;
        while(chindex < words[lineno].length()){
            if(at() == '"'){
                quotec++;
                if(quotec == 2){Pusher(s,TT_STRING);s="";break;}
                chindex++;
            }
            if(at() == '"'){
                quotec++;
                if(quotec == 2){Pusher(s,TT_STRING);s="";break;}
            }
            s+=at();
            chindex++;
        }
        if(quotec < 2){StrConErr(CreateToken(TT_STRING,s,lineno));/*STRING MUST BE CLOSED BY DOUBLE QUOTE!*/}
    }
void Lexer::MakeTokens(){
        while(lineno < words.size()){
            while(chindex < words[lineno].length()){
                //NUMBER
                if(IsNum(at())){MakeNumber();}
                //STRING
                if(at() == '"'){MakeString();}
                //IDENTIFIER / KEYWORD
                if(IsIden(at())){MakeWord();}
                //SYMBOLS
                if(at() == '='){
                    if(tokens.back().type == TT_EQUAL){
                        tokens.erase(tokens.end());
                        Pusher("==",TT_EE);
                    }
                    else if(tokens.back().type == TT_GT){
                        tokens.erase(tokens.end());
                        Pusher(">=",TT_GTE);
                    }
                    else if(tokens.back().type == TT_LT){
                        tokens.erase(tokens.end());
                        Pusher("<=",TT_LTE);
                    }
                    else if(tokens.back().type == TT_NOT){
                        tokens.erase(tokens.end());
                        Pusher("!=",TT_NE);
                    }
                    else{
                        Pusher(at(),TT_EQUAL);
                    }
                }
                if(at() == '<'){Pusher(at(),TT_LT);}
                if(at() == '>'){Pusher(at(),TT_GT);}
                if(at() == '!'){Pusher(at(),TT_NOT);}
                if(at() == '{'){Pusher(at(),TT_LCURLY);}
                if(at() == '}'){Pusher(at(),TT_RCURLY);}
                if(at() == '['){Pusher(at(),TT_LBOX);}
                if(at() == ']'){Pusher(at(),TT_RBOX);}
                if(at() == '-'){Pusher(at(),TT_MINUS);}
                if(at() == '+'){Pusher(at(),TT_PLUS);}
                if(at() == '*'){Pusher(at(),TT_MUL);}
                if(at() == '/'){Pusher(at(),TT_DIV);}
                if(at() == '%'){Pusher(at(),TT_MOD);}
                if(at() == '^'){Pusher(at(),TT_POW);}
                if(at() == '('){Pusher(at(),TT_LPAREN);}
                if(at() == ')'){Pusher(at(),TT_RPAREN);}
                if(at() == ':'){Pusher(at(),TT_COLON);}
                if(at() == ';'){Pusher(at(),TT_SEMICOLON);}
                //COMMENTS
                if(at() == '#'){break;}
                chindex++;
            }
            chindex = 0;
            lineno++;
        }
    }
std::vector<Token> Lexer::GetTokens(){
        //for(Token t:tokens){cout << " ~ " << t.val+", " << t.type;}
        return tokens;
    }
