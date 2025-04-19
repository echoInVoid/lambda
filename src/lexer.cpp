#include "lexer.hpp"

#include <vector>
#include <string>
#include <sstream>
#include "format.hpp"

namespace lexer
{

    LexerError::LexerError() : message("An unknown lexer error occurred."), pos(-1) {}

    LexerError::LexerError(std::string m, int p=-1)
    {
        message = m;
        pos = p;
    }

    const char* LexerError::what()
    {
        return message.c_str();
    }

    const int LexerError::where()
    {
        return pos;
    }

    Lexer::Lexer()
    {
        status = Status::free;
        pos = 0;
    }

    std::vector<Token> Lexer::getTokens()
    {
        return tokens;
    }

    void Lexer::setInput(std::string input)
    {
        inputStream = std::istringstream(input);
    }

    void Lexer::emitToken(TokenType type, std::string data)
    {
        tokens.push_back(Token{type, data});
    }

    void Lexer::emitCurSymbol()
    {
        status = Status::free;
        emitToken(TokenType::symbol, curSymbol);
        curSymbol.clear();
    }

    const bool Lexer::isReserved(char cur)
    {
        return reserved.find(cur) != std::string::npos;
    }

    const TokenType Lexer::getTokenType(char cur)
    {
        return (TokenType)(reserved.find(cur) + 1);
    }

    void Lexer::panic(char cur)
    {
        std::string message = util::Format(
            "Lexer: Unexpected char '{0}' at column {1}.",
            cur,
            pos+1
        );
        throw LexerError(message, pos);
    }

    void Lexer::scan()
    {
        char cur;
        while (!inputStream.get(cur).eof())
        {
            switch (status)
            {
                case Status::free:
                if (isReserved(cur))
                    emitToken(getTokenType(cur), std::string{cur});
                else if (isalpha(cur))
                {
                    curSymbol.push_back(cur);
                    status = Status::inSymbol;
                }
                else if (cur == ' ')
                    /* ignore it */;
                else
                    panic(cur);
                break;
                
                case Status::inSymbol:
                if (isalnum(cur))
                    curSymbol.push_back(cur);
                else if (isReserved(cur))
                {
                    emitCurSymbol();
                    emitToken(getTokenType(cur), std::string{cur});
                }
                else if (cur == ' ')
                    emitCurSymbol();
                else
                    panic(cur);
            }

            pos++;
        }

        if (!curSymbol.empty())
            emitCurSymbol();
    }

};