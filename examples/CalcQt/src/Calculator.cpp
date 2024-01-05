#include "Calculator.hpp"

#include <QRegularExpression>

QString Calculator::calculate(const QString& expression) const {
    int result = 0;
    char operation = '+';
    const QRegularExpression regexp("(\\d+)");
    QRegularExpressionMatchIterator matches = regexp.globalMatch(expression);

    while (matches.hasNext()) {
        const auto match = matches.next();
        const int value = match.captured(1).toInt();

        switch (operation) {
        case '+':
            result += value;
            break;
        case '-':
            result -= value;
            break;
        }

        const int pos = match.capturedEnd();
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
