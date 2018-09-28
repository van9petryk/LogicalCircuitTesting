#ifndef PARSEERROR_H
#define PARSEERROR_H

#include <stdexcept>
#include <QByteArray>
#include "token.h"

class ParseError : public std::runtime_error {
public:
    ParseError(const QByteArray &err, const Token &tok) :
        std::runtime_error(QByteArray("at row " + QByteArray::number(tok.row) + ", column " +
                                      QByteArray::number(tok.col) + ": " + err).toStdString()) {}
    ParseError(const QByteArray &err) : std::runtime_error(err.toStdString()) {}
};

#endif // PARSEERROR_H
