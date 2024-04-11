#include "cucumber-cpp/internal/drivers/QtTestDriver.hpp"

#include <QtTest/QtTest>
#include <QTextStream>
#include <QTemporaryFile>

namespace cucumber {
namespace internal {

const InvokeResult QtTestStep::invokeStepBody() {
    QString file_name;
    {
        QTemporaryFile file(QString("%1/%2_%3")
                                .arg(
                                    QDir::tempPath(),
                                    qApp->applicationName().isEmpty() ? "qt_temp"
                                                                      : qApp->applicationName()
                                )
                                .arg(qApp->applicationPid()));
        if (!file.open()) {
            return InvokeResult::failure("Unable to open temporary file needed for this test");
        }
        file_name = file.fileName() + ".txt";
    }

    QtTestObject testObject(this);
    int returnValue = QTest::qExec(
        &testObject,
        QStringList() << "test"
                      << "-o" << file_name
    );

    const auto ok = returnValue == 0;
    if (ok) {
        QFile::remove(file_name);
        return InvokeResult::success();
    } else {
        std::string error_text;
        QFile file(file_name);
        file.open(QIODevice::ReadOnly);
        error_text = QString::fromLocal8Bit(file.readAll()).toStdString();
        file.close();
        file.remove();
        return InvokeResult::failure(error_text);
    }
}

}
}
