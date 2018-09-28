#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    sectionFormat.setForeground(Qt::darkBlue);
    QStringList keywordPatterns;
    keywordPatterns << "\\bINPUTS\\b" << "\\bOUTPUTS\\b" << "\\bELEMENTS\\b" << "\\bLINKS\\b"
                    << "\\bTASK\\b" << "\\bMALFUNCTIONS\\b" << "\\bTESTS\\b" << "\\bDIAGNOSTIC\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = sectionFormat;
        highlightingRules.append(rule);
    }

    elementTypeFormat.setForeground(Qt::darkMagenta);
    QStringList elementPatterns;
    elementPatterns << "\\bOR\\b" << "\\bNOR\\b" << "\\bAND\\b"
                    << "\\bNAND\\b" << "\\bXOR\\b" << "\\bXNOR\\b"
                    << "\\NOT\\b" << "\\bEQV\\b";
    foreach (const QString &pattern, elementPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = elementTypeFormat;
        highlightingRules.append(rule);
    }
    highlightingRules.append(rule);

    binaryConstantFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\\b[01]+\\b");
    rule.format = binaryConstantFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegExp("#[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}
