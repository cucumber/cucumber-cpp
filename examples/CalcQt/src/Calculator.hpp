#pragma once

#include <QObject>

class Calculator : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;

public slots:
    void number(quint8);
    void add();
    void subtract();
    void calculate();
    void clear();

signals:
    void updateDisplay(QString);

private:
    QString expression{};
    QString calculate(const QString& expression) const;
};
