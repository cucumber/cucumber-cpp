#include <QApplication>

#include "Calculator.hpp"
#include "CalculatorWidget.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Qt Calculator");
    Calculator calculator{};
    CalculatorWidget widget{&calculator};
    widget.show();
    return app.exec();
}
