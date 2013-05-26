#include <QApplication>

#include "CalculatorWidget.h"

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(application);
    QApplication app(argc, argv);
    app.setApplicationName("Qt Calculator");
    CalculatorWidget widget;
    widget.show();
    return app.exec();
}
