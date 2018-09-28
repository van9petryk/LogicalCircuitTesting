#ifndef PARSER_H
#define PARSER_H

#include <QList>
#include "lexer.h"
#include "parsestructures.h"

class Parser
{
public:
    Parser(const QByteArray &description);
    CircuitDescription getCircuitDescription() { return circuit; }
    QMap <QByteArray, TaskDescription> getTasks() { return tasks; }
private:
    Lexer lex;

    CircuitDescription circuit;
    QMap <QByteArray, TaskDescription> tasks;

    Token parseToken(Token::Type t);
    QList <Token> parseCommaList(Token::Type t);

    void checkDelimiter(const QByteArray &delim);
    void parseDelimiter(const QByteArray &delim);
    void parseSectionName(const QByteArray &sectionName);
    void checkSectionEnd() { checkDelimiter(";"); }
    Token parseTaskName();

    void parseInputs();
    void parseOutputs();
    void parseElements();
    void parseLinks();

    void parseMalfunctions(TaskDescription &task);
    bool parseTests(TaskDescription &task);
    bool parseDiagnostic(TaskDescription &task);

    void parseCircuit();
    void parseTask();

    QByteArray setToStr(const QSet<QByteArray> &s);
};

#endif // PARSER_H
