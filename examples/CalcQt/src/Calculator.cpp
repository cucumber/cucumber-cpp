#include "Calculator.hpp"

QString Calculator::calculate(const QString& expression) const {
    int result = 0;
    char operation = '+';
    QRegExp regexp("(\\d+)");
    int pos = 0;
    while ((pos = regexp.indexIn(expression, pos)) != -1) {
        int value = regexp.cap(1).toInt();
        switch (operation) {
        case '+':
            result += value;
            break;
        case '-':
            result -= value;
            break;
        }
        pos += regexp.matchedLength();
        if (pos < expression.length()) {
            operation = expression.at(pos).toLatin1();
        }
    }
    return QString::number(result);
}

void Calculator::number(quint8 number) {
    expression += QString::number(number);
    emit updateDisplay(expression);
}

void Calculator::add() {
    expression += "+";
    emit updateDisplay(expression);
}

void Calculator::subtract() {
    expression += "-";
    emit updateDisplay(expression);
}

void Calculator::calculate() {
    expression = calculate(expression);
    emit updateDisplay(expression);
}

void Calculator::clear() {
    expression.clear();
    emit updateDisplay(expression);
}
