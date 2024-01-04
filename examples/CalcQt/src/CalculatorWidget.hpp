#pragma once

class QLabel;
class QPushButton;
class QSignalMapper;

#include <QString>
#include <QWidget>

class CalculatorWidget : public QWidget {
    Q_OBJECT

public:
    CalculatorWidget(QWidget* parent = 0);

    QString display();

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

    QPushButton* keyclickedButton;

    QSignalMapper* signalMapper;

    int calculate(const QString& expression);

private Q_SLOTS:

    void addButtonClicked();
    void buttonClicked(int index);
    void calculateButtonClicked();
    void clearButtonClicked();
    void subtractButtonClicked();
};
