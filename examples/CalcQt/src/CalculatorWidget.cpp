#include "CalculatorWidget.hpp"

#include "Calculator.hpp"
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>

CalculatorWidget::CalculatorWidget(Calculator* calculator, QWidget* parent) :
    QWidget(parent) {
    QGridLayout* layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);

    QSizePolicy policy = sizePolicy();

    displayLabel = new QLabel(this);
    layout->addWidget(displayLabel, 0, 0, 1, 3);
    displayLabel->setAutoFillBackground(true);
    displayLabel->setBackgroundRole(QPalette::Base);
    displayLabel->setAlignment(Qt::AlignRight);
    policy = displayLabel->sizePolicy();
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    displayLabel->setSizePolicy(policy);
    QObject::connect(calculator, &Calculator::updateDisplay, displayLabel, &QLabel::setText);

    QPushButton* button = new QPushButton(QString::number(0), this);
    QObject::connect(button, &QPushButton::clicked, calculator, [calculator] {
        calculator->number(0);
    });
    layout->addWidget(button, 4, 1);
    digitButtons.push_back(button);
    for (unsigned int i = 1; i < 10; ++i) {
        QPushButton* button = new QPushButton(QString::number(i), this);
        QObject::connect(button, &QPushButton::clicked, calculator, [i, calculator] {
            calculator->number(i);
        });
        layout->addWidget(button, 1 + (9 - i) / 3, (i - 1) % 3);
        digitButtons.push_back(button);
    }

    clearButton = new QPushButton("C", this);
    layout->addWidget(clearButton, 1, 4);
    QObject::connect(clearButton, &QPushButton::clicked, calculator, [calculator] {
        calculator->clear();
    });

    additionButton = new QPushButton("+", this);
    layout->addWidget(additionButton, 2, 4);
    QObject::connect(additionButton, &QPushButton::clicked, calculator, [calculator] {
        calculator->add();
    });

    subtractionButton = new QPushButton("-", this);
    layout->addWidget(subtractionButton, 3, 4);
    QObject::connect(subtractionButton, &QPushButton::clicked, calculator, [calculator] {
        calculator->subtract();
    });

    calculateButton = new QPushButton("=", this);
    layout->addWidget(calculateButton, 4, 4);
    QObject::connect(calculateButton, &QPushButton::clicked, calculator, [calculator] {
        calculator->calculate();
    });
}

void CalculatorWidget::keyPressEvent(QKeyEvent* event) {
    keyclickedButton = nullptr;
    int key = event->key();
    if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        keyclickedButton = digitButtons[key - Qt::Key_0];
    } else {
        switch (key) {
        case Qt::Key_Plus:
            keyclickedButton = additionButton;
            break;
        case Qt::Key_Minus:
            keyclickedButton = subtractionButton;
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Equal:
            keyclickedButton = calculateButton;
            break;
        case Qt::Key_Escape:
            keyclickedButton = clearButton;
            break;
        }
    }
    if (keyclickedButton) {
        keyclickedButton->click();
        keyclickedButton->setDown(true);
    }
}

void CalculatorWidget::keyReleaseEvent(QKeyEvent* event) {
    Q_UNUSED(event)
    if (keyclickedButton) {
        keyclickedButton->setDown(false);
        keyclickedButton = nullptr;
    }
}

void CalculatorWidget::updateDisplay(QString text) {
    displayLabel->setText(text);
}
