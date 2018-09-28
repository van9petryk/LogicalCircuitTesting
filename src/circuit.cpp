#include "circuit.h"

Circuit::~Circuit()
{
    foreach (Component* component, components)
        delete component;
}

Circuit::Circuit(const CircuitDescription &cd)
{
    foreach (const QByteArray &inputName, cd.inputs) {
        Input *input = new Input(inputName);
        inputs.append(input);
        components.insert(inputName, input);
    }

    foreach (const QByteArray &elementName, cd.elements.keys()) {
        LogicElement *element = new LogicElement(elementName, cd.elements[elementName].logicFunc);
        elements.append(element);
        components.insert(elementName, element);
    }

    for (int i = 0; i < elements.size(); i++)
        foreach (const QByteArray &componentName, cd.elements[elements[i]->getName()].inputs)
            elements[i]->addInput(components[componentName]);

    foreach (const QByteArray &output, cd.outputs.keys())
        outputs.insert(output, components[cd.outputs[output]]);

    doRanging();
}

QMap<QByteArray, QByteArray> Circuit::simulate(const Malfunction &malfunc)
{
    components[malfunc.name]->setMalfunction(malfunc.val);
    const auto &result =  simulate();
    components[malfunc.name]->unsetMalfunction();
    return result;
}

QMap<QByteArray, QByteArray> Circuit::simulate(const QList<QByteArray> &inputsValues, const Malfunction &malfunc)
{
    components[malfunc.name]->setMalfunction(malfunc.val);
    const auto &result = simulate(inputsValues);
    components[malfunc.name]->unsetMalfunction();
    return result;
}

QMap<QByteArray, QByteArray> Circuit::simulate()
{
    QMap <QByteArray, QByteArray> result;
    for (int i = 0; i < pow(2, inputs.count()); i++) {
        setInputsValue(i);
        eval();
        foreach (const QByteArray &out, outputs.keys())
            result[out] += outputs[out]->getValue() + '0';
    }
    return result;
}

QMap<QByteArray, QByteArray> Circuit::simulate(const QList<QByteArray> &inputsValues)
{
    QMap <QByteArray, QByteArray> result;
    foreach (const QByteArray &inputsValue, inputsValues) {
        setInputsValue(inputsValue);
        eval();
        foreach (const QByteArray &out, outputs.keys())
            result[out] += outputs[out]->getValue() + '0';
    }
    return result;
}

QList<QByteArray> Circuit::getNameInputs()
{
    QList <QByteArray> inputsName;
    for (int i = 0; i < inputs.size(); i++)
        inputsName += inputs[i]->getName();
    return inputsName;
}

void Circuit::setInputsValue(const QByteArray &inputsValue)
{
    if (inputsValue.count() != inputs.count())
        throw std::runtime_error("кол-во входов не соответсвует длине вектора");
    for (int i = 0; i < inputs.size(); i++)
        inputs[i]->setValue(inputsValue[i] - '0');
}

void Circuit::setInputsValue(int inputsValue)
{
    for (int i = 0; i < inputs.count(); i++)
        inputs[inputs.count() - 1 - i]->setValue(inputsValue & (1 << i));
}

void Circuit::eval()
{
    foreach (LogicElement *element, elements)
        element->eval();
}

void Circuit::doRanging()
{
    QSet<LogicElement*> currentElements;
    QHash <LogicElement*, QList <LogicElement*>> elementOutputs;

    foreach (LogicElement* element, elements) {
        foreach (Component* component, element->getInputs()) {
            if (component->type() == Component::INPUT)
                currentElements.insert(element);
            else
                elementOutputs[static_cast<LogicElement*>(component)] += element;
        }
    }

    QList <LogicElement*> rangedElements;
    int i = 1000;
    do {
        foreach (LogicElement* element, currentElements) {
            bool isAllInputsKnown = true;
            foreach (Component* component , element->getInputs())
                if (component->type() != Component::INPUT)
                    if (!rangedElements.contains(static_cast<LogicElement*>(component))) {
                        isAllInputsKnown = false;
                        break;
                    }
            if (isAllInputsKnown) {
                rangedElements += element;
                foreach (LogicElement* out, elementOutputs[element])
                    currentElements.insert(out);
                currentElements.remove(element);
            }
        }

        if (--i == 0)
            throw std::runtime_error("Не удалось ранжировать элементы. Схема не комбинационная.");
    } while (!currentElements.isEmpty());

    elements = rangedElements;
}
