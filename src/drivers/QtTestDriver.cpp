#include "cucumber-cpp/internal/drivers/QtTestDriver.hpp"

#include <QtTest/QtTest>
#include <QTextStream>
#include <QTemporaryFile>

namespace cucumber {
namespace internal {

const InvokeResult QtTestStep::invokeStepBody() {
    QString file_name;
    {
        QTemporaryFile file;
        if (!file.open()) {
            return InvokeResult::failure("Unable to open temporary file needed for this test");
        }
        file.close();
    }

    file_name += ".txt";

    QtTestObject testObject{this};
    const QStringList args{"test", "-o", file_name + ",tap"};
    const int returnValue = QTest::qExec(&testObject, args);
    const bool ok = returnValue == 0;
    std::string error_text;
    QFile file( file_name );
    if(!ok) {
      file.open( QIODevice::ReadOnly );
      error_text = QString::fromLocal8Bit( file.readAll() ).toStdString();
    }
    //file.remove();
    return ok ?  InvokeResult::success() :  InvokeResult::failure( error_text );
}

}
}
