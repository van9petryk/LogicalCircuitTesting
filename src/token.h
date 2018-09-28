#ifndef TOKEN_H
#define TOKEN_H

#include <QByteArray>

struct Token {
    int row;
    int col;
    QByteArray value;
    enum Type {SECTION, ELEMENT_TYPE, DELIMITER, IDENTIFIER, BINARY_CONSTANT, END} type;
    static QByteArray typeStr(Token::Type t) {
        if (t == SECTION)
            return "SECTION";
        if (t == ELEMENT_TYPE)
            return "ELEMENT_TYPE";
        if (t == DELIMITER)
            return "DELIMITER";
        if (t == IDENTIFIER)
            return "IDENTIFIER";
        if (t == BINARY_CONSTANT)
            return "BINARY_CONSTANT";
        return "END_OF_FILE";
    }
};

#endif // TOKEN_H
