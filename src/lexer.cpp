#include "lexer.h"

Lexer::Lexer(const QByteArray &description) : text(description)
{
    endidx = text.length();
    skip();
}

Token Lexer::nextToken()
{
    token.row = row;
    token.col = col;

    if (idx == endidx) {
        token.type = Token::END;
        return token;
    }

    if (delimiters.contains(text.at(idx))) {
        token.value = text.mid(idx, 1);
        token.type = Token::DELIMITER;
        idx++; col++;
    }
    else if (isalnum(text.at(idx))) {
        QByteArray value;
        do {
            value += text.at(idx);
            idx++; col++;
        } while(idx < endidx && isalnum(text.at(idx)));

        token.value = value;

        if(QRegExp("[01]+").exactMatch(value))
            token.type = Token::BINARY_CONSTANT;
        else {
            if (sections.contains(value))
                token.type = Token::SECTION;
            else if (elements.contains(value))
                token.type = Token::ELEMENT_TYPE;
            else if (QRegExp("[A-Za-z][\\w]*").exactMatch(value))
                token.type = Token::IDENTIFIER;
            else
                throw ParseError("неизвестный тип лексемы '" + token.value + "'", token);
        }
    }
    else
        throw ParseError("неразрешенный символ '" + text.mid(idx, 1) + "'", token);

    skip();

    return token;
}

void Lexer::skip()
{
    while (idx < endidx)
    {
        if (isspace(text[idx]))
            skipSpace();
        else if (text[idx] == '#')
            skipComment();
        else
            break;
    }
}

void Lexer::skipSpace()
{
    if (text[idx] == '\n')
    {
        col = 1;
        row++;
    }
    else
        col++;
    idx++;
}

void Lexer::skipComment()
{
    do {
        idx++;
        col++;
    } while (idx < endidx && text[idx] != '\n');
}
