#include <QApplication>

#include "CalculatorWidget.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Qt Calculator");
    CalculatorWidget widget;
    widget.show();
    return app.exec();
}
