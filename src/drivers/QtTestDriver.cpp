#include <cucumber-cpp/internal/drivers/QtTestDriver.hpp>

#include <QtTest/QtTest>
#include <QString>
#include <QTextStream>
#include <QTemporaryFile>
#include <QBuffer>
#include <QObject>
#include <QFile>
#include <cstdio>
#include "cucumber-cpp/internal/utils/qtCapture.hpp"

namespace cucumber {
namespace internal {

class QtTestObject: public QObject {
    Q_OBJECT
public:
    QtTestObject(QtTestStep* qtTestStep): step(qtTestStep) {}
    virtual ~QtTestObject() {}

protected:
    QtTestStep* step;

private slots:
    void test() const {
        step->body();
    }
};

const InvokeResult QtTestStep::invokeStepBody() {
    QtTestObject testObject(this);

    qtCapture::Init();
    qtCapture::BeginCapture();
    int returnValue = QTest::qExec(&testObject, 0, NULL);
    qtCapture::EndCapture();

    if(returnValue == 0)
        return InvokeResult::success();
    else
        return InvokeResult::failure(qtCapture::GetCapture());
}

}
}

#include "QtTestDriver.moc"
