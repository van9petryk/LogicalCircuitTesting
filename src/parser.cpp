#include "parser.h"

Parser::Parser(const QByteArray &description) : lex(description)
{
    try {
        parseCircuit();
        lex.nextToken();
        while (lex.hasNextToken()) {
            parseTask();
        }
    } catch (const ParseError &e) {
        if (lex.getToken().type == Token::END)
            throw ParseError(QByteArray(e.what()) + ", но был достигнут конец файла");
        else throw e;
    }

    if (lex.getToken().type != Token::END)
        throw ParseError("лишние символы " + lex.getToken().value, lex.getToken());
}

void Parser::parseCircuit()
{
    parseInputs();
    parseOutputs();
    parseElements();
    parseLinks();
}

void Parser::parseTask()
{
    const Token &taskName = parseTaskName();
    if (tasks.contains(taskName.value))
        throw ParseError("'" + taskName.value + "' уже описан ранее", taskName);
    TaskDescription task;
    parseMalfunctions(task);
    if (parseTests(task))
        if (parseDiagnostic(task))
            lex.nextToken();
    tasks.insert(taskName.value, task);
}

Token Parser::parseTaskName()
{
    if (lex.getToken().value != "TASK")
        throw ParseError("ожидался раздел 'TASK'", lex.getToken());
    return parseToken(Token::IDENTIFIER);
}

Token Parser::parseToken(Token::Type t)
{
    if (lex.nextToken().type != t)
        throw ParseError("ожидался токен '" + Token::typeStr(t) + "'", lex.getToken());
    return lex.getToken();
}

QList<Token> Parser::parseCommaList(Token::Type t)
{
    QList <Token> l;
    do {
       l.append(parseToken(t));
    } while(lex.nextToken().value == ",");
    return l;
}

void Parser::parseDelimiter(const QByteArray &delim)
{
    if (lex.nextToken().value != delim)
        throw ParseError("ожидался разделитель '" + delim + "'", lex.getToken());
}

void Parser::checkDelimiter(const QByteArray &delim)
{
    if (lex.getToken().value != delim)
        throw ParseError("ожидался разделитель '" + delim + "'", lex.getToken());
}

void Parser::parseSectionName(const QByteArray &sectionName)
{
    if (lex.nextToken().value != sectionName)
        throw ParseError("ожидался раздел '" + sectionName + "'", lex.getToken());
}

void Parser::parseInputs()
{
    parseSectionName("INPUTS");
    parseDelimiter(":");

    const auto &inputs = parseCommaList(Token::IDENTIFIER);
    foreach(const Token &t, inputs) {
        if (circuit.inputs.contains(t.value))
            throw ParseError("'" + t.value + "' уже объявлен в разделе INPUTS", t);
        else
            circuit.inputs.append(t.value);
    }

    checkSectionEnd();
}

void Parser::parseOutputs()
{
    parseSectionName("OUTPUTS");
    parseDelimiter(":");

    const auto &outputs = parseCommaList(Token::IDENTIFIER);

    foreach(const Token &t, outputs) {
        if (circuit.inputs.contains(t.value))
            throw ParseError("'" + t.value + "' уже объявлен в разделе INPUTS", t);
        else if (circuit.outputs.contains(t.value))
            throw ParseError("'" + t.value + "' уже объявлен в разделе OUTPUTS", t);
        else
            circuit.outputs.insert(t.value, QByteArray());
    }

    checkSectionEnd();
}

void Parser::parseElements()
{
    parseSectionName("ELEMENTS");
    parseDelimiter(":");

    do {
        const auto &elements = parseCommaList(Token::IDENTIFIER);
        checkDelimiter("=");
        const Token &logicFunc = parseToken(Token::ELEMENT_TYPE); //repair

        foreach (const Token &t, elements) {
            if (circuit.inputs.contains(t.value))
                throw ParseError("'" + t.value + "' уже объявлен в разделе INPUTS", t);
            else if (circuit.outputs.contains(t.value))
                throw ParseError("'" + t.value + "' уже объявлен в разделе OUTPUTS", t);
            else if (circuit.elements.contains(t.value))
                throw ParseError("'" + t.value + "' уже объявлен в разделе ELEMENTS", t);
            else
                circuit.elements.insert(t.value, LogicElementDescription(logicFunc.value));
        }

    } while (lex.nextToken().value == ",");

    checkSectionEnd();
}

void Parser::parseLinks()
{
    parseSectionName("LINKS");
    parseDelimiter(":");

    auto withoutConnIn = circuit.elements.keys().toSet() + circuit.outputs.keys().toSet();
    auto withoutConnOut = circuit.elements.keys().toSet() + circuit.inputs.toSet();

    do {
        const Token &withIn = parseToken(Token::IDENTIFIER);
        bool isElement;
        if (circuit.elements.contains(withIn.value))
            isElement = true;
        else if (circuit.outputs.contains(withIn.value))
            isElement = false;
        else
            throw ParseError("'" + withIn.value + "' не объявлен в разделе ELEMENTS/OUTPUTS", withIn);
        if (!withoutConnIn.contains(withIn.value))
            throw ParseError("двойное описание '" + withIn.value + "'", withIn);
        withoutConnIn.remove(withIn.value);

        bool oneInput = true;
        if (isElement)
            if (circuit.elements[withIn.value].logicFunc != EQV && circuit.elements[withIn.value].logicFunc!= NOT)
                oneInput = false;

        parseDelimiter("(");

        const auto &withOut = parseCommaList(Token::IDENTIFIER);

        if (oneInput && withOut.size() != 1)
            throw ParseError("'" + withIn.value + "' должен быть связан с одним элементом", withIn);
        else if (!oneInput && withOut.size() < 2)
            throw ParseError("'" + withIn.value + "' должен быть связан с двумя или больше элементами", withIn);

        QList <QByteArray> inputs;
        foreach (const Token &t, withOut) {
            if (circuit.elements.contains(t.value)) {
                if (t.value == withIn.value)
                    throw ParseError("выход элемента '" + t.value + "' подключен к его в входу", t);
            }
            else if (!circuit.inputs.contains(t.value))
                throw ParseError("'" + t.value + "' не объявлен в разделе INPUTS/ELEMENTS", t);
            withoutConnOut.remove(t.value);
            inputs.append(t.value);
        }

        if (isElement)
            circuit.elements[withIn.value].inputs = inputs;
        else
            circuit.outputs[withIn.value] = inputs.first();

        checkDelimiter(")");

    } while (lex.nextToken().value == ",");

    QByteArray withoutConn;
    if (withoutConnIn.size() != 0)
        withoutConn = "Входы " + setToStr(withoutConnIn) + " не подключены к другим элементам\n";
    if (withoutConnOut.size() != 0)
        withoutConn += "Выходы " + setToStr(withoutConnOut) + " не подключены к другим элементам\n";

    if (withoutConn.size() != 0)
        throw ParseError(withoutConn);

    checkSectionEnd();
}

void Parser::parseMalfunctions(TaskDescription &task)
{
    parseSectionName("MALFUNCTIONS");
    parseDelimiter(":");

    do {
        const auto &malfuncElem = parseCommaList(Token::IDENTIFIER);

        foreach (const Token &t, malfuncElem)
            if (!circuit.inputs.contains(t.value) && !circuit.elements.contains(t.value))
                throw ParseError("'" + t.value + "' не объявлен в разделе INPUTS/ELEMENTS", t);

        checkDelimiter("=");

        if (parseToken(Token::BINARY_CONSTANT).value.size() != 1)
            throw ParseError("ожидалась двоичная константа '0' или '1'", lex.getToken());
        bool constant = lex.getToken().value.toInt();
        foreach (const Token &t, malfuncElem) {
            Malfunction malfunc(t.value, constant);
            if (task.malfunctions.contains(malfunc))
                throw ParseError("'" + malfunc.str() + "' уже объявлен ранее", t);
            task.malfunctions.append(malfunc);
        }

    } while (lex.nextToken().value == ",");

    checkSectionEnd();
}

bool Parser::parseTests(TaskDescription &task)
{
    try {
        parseSectionName("TESTS");
    } catch (const ParseError &e) {
        return false;
    }
    parseDelimiter(":");

    do {
        const Token &testName = parseToken(Token::IDENTIFIER);
        if (task.tests.contains(testName.value))
            throw ParseError("'" + testName.value + "' уже описан в разделе TESTS", testName);

        parseDelimiter("(");

        const auto &testValues = parseCommaList(Token::BINARY_CONSTANT);
        QList <QByteArray> test;
        foreach(const Token &t, testValues) {
            if (t.value.size() != circuit.inputs.size())
                throw ParseError("размерность входного сигнала " + t.value +
                                 " не равна количеству входов", t);
            test += t.value;
        }

        task.tests.insert(testName.value, TestDescription(test));

        checkDelimiter(")");
    } while(lex.nextToken().value == ",");

    checkSectionEnd();

    return true;
}

bool Parser::parseDiagnostic(TaskDescription &task)
{
    try {
        parseSectionName("DIAGNOSTIC");
    }
    catch (const ParseError &e) {
        return false;
    }
    parseDelimiter(":");

    do {
        const Token &testName = parseToken(Token::IDENTIFIER);
        if (!task.tests.contains(testName.value))
            throw ParseError("'" + testName.value + "' не описан в разделе TESTS", testName);
        auto &test = task.tests[testName.value];
        if (test.diagnostic.size() != 0)
            throw ParseError("'" + testName.value + "' уже описан в разделе DIAGNOSTIC", testName);

        parseDelimiter("(");

        do {
            const Token &out = parseToken(Token::IDENTIFIER);
            if (!circuit.outputs.contains(out.value))
                throw ParseError("'" + out.value + "' не описан в разделе OUTPUTS", out);
            if (test.diagnostic.contains(out.value))
                throw ParseError("'" + out.value + "' уже описан в тесте " + testName.value, out);

            parseDelimiter("=");
            parseDelimiter("[");

            const auto &expectedValues = parseCommaList(Token::BINARY_CONSTANT);
            if (expectedValues.size() != task.malfunctions.size())
                throw ParseError("количество выходных сигналов не равна количеству неисправностей");
            foreach (const Token &t, expectedValues) {
                if (t.value.size() != test.test.size())
                    throw ParseError("размерность выходного сигнала '" + t.value + "' не равна количеству тестов", t);
                test.diagnostic[out.value] += t.value;
            }

            checkDelimiter("]");
        } while (lex.nextToken().value == ",");

        checkDelimiter(")");

        if (circuit.outputs.size() != test.diagnostic.size()) {
            const auto &outMissed = circuit.outputs.keys().toSet() - test.diagnostic.keys().toSet();
            throw ParseError("выходные сигналы для " + setToStr(outMissed) + " не описаны");
        }

    } while(lex.nextToken().value == ",");

    checkSectionEnd();

    return true;
}

QByteArray Parser::setToStr(const QSet<QByteArray> &s)
{
    QByteArray str;
    foreach (const QByteArray &ba, s)
        str += "'" + ba + "',";
    return str;
}
