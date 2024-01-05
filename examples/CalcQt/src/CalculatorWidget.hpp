#pragma once

class QLabel;
class QPushButton;
class Calculator;

#include <QString>
#include <QWidget>

class CalculatorWidget : public QWidget {
    Q_OBJECT

public:
    CalculatorWidget(Calculator* calculator, QWidget* parent = 0);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    QLabel* displayLabel;
    QVector<QPushButton*> digitButtons;
    QPushButton* additionButton;
    QPushButton* calculateButton;
    QPushButton* clearButton;
    QPushButton* subtractionButton;

    QPushButton* keyclickedButton{};

private Q_SLOTS:
    void updateDisplay(QString);
};
