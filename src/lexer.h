#ifndef LEXER_H
#define LEXER_H

#include <QByteArray>
#include <QSet>
#include "token.h"
#include "parseerror.h"

class Lexer
{
public:
    Lexer(const QByteArray &description);
    Token getToken() { return token; }
    Token nextToken();
    bool hasNextToken() { return idx < endidx; }
private:
    Token token;
    const QByteArray &text;
    const QByteArray delimiters = "(),;:=[]";
    const QSet <QByteArray> sections = {"INPUTS", "OUTPUTS", "ELEMENTS", "LINKS", "MALFUNCTIONS", "TESTS", "DIAGNOSTIC", "TASK"};
    const QSet <QByteArray> elements = {"AND", "NAND", "OR", "NOR", "XOR", "XNOR", "EQV", "NOT"};
    uint col = 1;
    uint row = 1;
    uint idx = 0;
    uint endidx;

    void skip();
    void skipSpace();
    void skipComment();
};

#endif // LEXER_H
