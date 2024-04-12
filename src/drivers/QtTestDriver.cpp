#include "cucumber-cpp/internal/drivers/QtTestDriver.hpp"

#include <QtTest/QtTest>
#include <QTextStream>
#include <QTemporaryFile>

#include <iostream>

namespace cucumber {
namespace internal {

// wraps the QTemporaryFile creation
// on Windows the file could not be written as long as QTemporaryFile owner of the file.

class TemporaryFileWrapper {
public:
    static TemporaryFileWrapper create() {
        QTemporaryFile tempFile(
            QString("%1/%2_%3")
                .arg(
                    QDir::tempPath(),
                    qApp->applicationName().isEmpty() ? "qt_temp"
                                                      : qApp->applicationName()
                )
                .arg(qApp->applicationPid())
        );

        if (!tempFile.open()) {
            return {};
        }

        return {tempFile.fileName() + ".txt"};
    }

    TemporaryFileWrapper() :
        filename{} {
    }

    TemporaryFileWrapper(QString filename) :
        filename{filename} {
    }

    ~TemporaryFileWrapper() {
        QFile::remove(filename);
    }

    bool exists() const {
        return !filename.isEmpty();
    }

    QString name() const {
        return filename;
    }

    QString read() const {
        QFile file{filename};
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return QString();
        QTextStream in(&file);
        return in.readAll();
    }

private:
    QString filename;
};

const InvokeResult QtTestStep::invokeStepBody() {
    const auto file = TemporaryFileWrapper::create();
    if (!file.exists()) {
        return InvokeResult::failure("Unable to open temporary file needed for this test");
    }

    QtTestObject testObject(this);
    int returnValue = QTest::qExec(
        &testObject,
        QStringList() << "test"
                      << "-o" << file.name()
    );

    if (returnValue == 0) {
        return InvokeResult::success();
    } else {
        return InvokeResult::failure(file.read().toLocal8Bit());
    }
}

}
}
