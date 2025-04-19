#include <iostream>
#include <string>
#include "lexer.hpp"

int main()
{
    std::string s = "()(@abc,cd:(sd dp12))";
    lexer::Lexer lex;
    lex.setInput(s);
    lex.scan();

    for (auto i : lex.getTokens())
        std::cout << '<' << i.type << ',' << i.data << ">\n";
    
    return 0;
}