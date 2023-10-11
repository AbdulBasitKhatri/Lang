#include "Evaluator.h"

int main()
{
    Lexer lexer("code.lang");
    std::vector<Token> tokens = lexer.GetTokens();
    Parser parser(tokens,"code.lang");
    Evaluator evaluator(parser.GetAST());
    return 0;
}
