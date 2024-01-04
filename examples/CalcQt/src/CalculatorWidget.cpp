#include "CalculatorWidget.hpp"

#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>

CalculatorWidget::CalculatorWidget(QWidget* parent) :
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

    QPushButton* button = new QPushButton(QString::number(0), this);
    QObject::connect(button, &QPushButton::clicked, this, [this] { buttonClicked(0); });
    layout->addWidget(button, 4, 1);
    digitButtons.push_back(button);
    for (unsigned int i = 1; i < 10; ++i) {
        QPushButton* button = new QPushButton(QString::number(i), this);
        QObject::connect(button, &QPushButton::clicked, this, [i, this] { buttonClicked(i); });
        layout->addWidget(button, 1 + (9 - i) / 3, (i - 1) % 3);
        digitButtons.push_back(button);
    }

    clearButton = new QPushButton("C", this);
    layout->addWidget(clearButton, 1, 4);
    QObject::connect(
        clearButton, &QPushButton::clicked, this, &CalculatorWidget::clearButtonClicked
    );

    additionButton = new QPushButton("+", this);
    layout->addWidget(additionButton, 2, 4);
    QObject::connect(
        additionButton, &QPushButton::clicked, this, &CalculatorWidget::addButtonClicked
    );

    subtractionButton = new QPushButton("-", this);
    layout->addWidget(subtractionButton, 3, 4);
    QObject::connect(
        subtractionButton, &QPushButton::clicked, this, &CalculatorWidget::subtractButtonClicked
    );

    calculateButton = new QPushButton("=", this);
    layout->addWidget(calculateButton, 4, 4);
    QObject::connect(
        calculateButton, &QPushButton::clicked, this, &CalculatorWidget::calculateButtonClicked
    );
}

int CalculatorWidget::calculate(const QString& expression) {
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
    return result;
}

QString CalculatorWidget::display() {
    return displayLabel->text();
}

void CalculatorWidget::keyPressEvent(QKeyEvent* event) {
    keyclickedButton = 0;
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
    if (0 != keyclickedButton) {
        keyclickedButton->click();
        keyclickedButton->setDown(true);
    }
}

void CalculatorWidget::keyReleaseEvent(QKeyEvent* event) {
    Q_UNUSED(event)
    if (0 != keyclickedButton) {
        keyclickedButton->setDown(false);
        keyclickedButton = 0;
    }
}

void CalculatorWidget::addButtonClicked() {
    displayLabel->setText(displayLabel->text() + "+");
}

void CalculatorWidget::buttonClicked(int index) {
    displayLabel->setText(displayLabel->text() + QString::number(index));
}

void CalculatorWidget::calculateButtonClicked() {
    displayLabel->setText(QString::number(calculate(displayLabel->text())));
}

void CalculatorWidget::clearButtonClicked() {
    displayLabel->setText("");
}

void CalculatorWidget::subtractButtonClicked() {
    displayLabel->setText(displayLabel->text() + "-");
}
