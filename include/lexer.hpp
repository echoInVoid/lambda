#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <exception>

namespace lexer
{

    enum TokenType
    {
        symbol,       // identifier
        lambda,       // λ
        dot,          // .
        leftBracket,  // (
        rightBracket, // )
        comma,        // ,
        colon,        // :
    };

    enum Status
    {
        free,         // 不在标识符中
        inSymbol      // 位于标识符中
    };

    class LexerError : std::exception
    {
        std::string message;
        int pos;

    public:
        LexerError();
        LexerError(std::string message, int pos);
        const char* what();
        const int where();
    };

    class Token
    {
    public:
        TokenType type;
        std::string data;
    };

    class Lexer
    {
        const std::string reserved{"@.(),:"};
        Status status;
        std::vector<Token> tokens;
        std::istringstream inputStream;
        std::string curSymbol;
        unsigned int pos;
        void emitToken(TokenType type, std::string data);
        void emitCurSymbol();
        void panic(char cur);
        const bool isReserved(char cur);
        const TokenType getTokenType(char cur);

    public:
        Lexer();
        ~Lexer() = default;

        void setInput(std::string);
        void scan();
        std::vector<Token> getTokens();
    };

};