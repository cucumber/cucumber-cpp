#include "cucumber-cpp/internal/drivers/QtTestDriver.hpp"

#include <QtTest/QtTest>
#include <QTextStream>
#include <QTemporaryFile>

namespace cucumber {
namespace internal {

const InvokeResult QtTestStep::invokeStepBody() {
    QTemporaryFile file;
    if (!file.open()) {
        return InvokeResult::failure("Unable to open temporary file needed for this test");
    }
    file.close();

    QtTestObject testObject(this);
    int returnValue = QTest::qExec(&testObject,
                                   QStringList() << "test"
                                                 << "-o"
                                                 << file.fileName());
    if (returnValue == 0) {
        return InvokeResult::success();
    } else {
        file.open();
        QTextStream ts(&file);
        return InvokeResult::failure(ts.readAll().toLocal8Bit());
    }
}

}
}

