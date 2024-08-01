#include "cucumber-cpp/internal/drivers/QtTestDriver.hpp"

#include <QTest>
#include <QTemporaryFile>
#include <QTextStream>

namespace cucumber {
namespace internal {

/**
 * Wraps the QTemporaryFile for Windows.
 *
 * On Windows. the file can not be written as long as QTemporaryFile keeps it open.
 */
class TemporaryFileWrapper {
public:
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
    const QString filename{getTmpFileName()};

    static QString getTmpFileName() {
        QTemporaryFile tempFile{};
        if (!tempFile.open()) {
            return {};
        }
        return tempFile.fileName() + ".txt";
    }
};

const InvokeResult QtTestStep::invokeStepBody() {
    const TemporaryFileWrapper file{};
    if (!file.exists()) {
        return InvokeResult::failure("Unable to open temporary file needed for this test");
    }

    QtTestObject testObject{this};
    const QStringList args{"test", "-o", file.name() + ",tap"};
    const int returnValue = QTest::qExec(&testObject, args);

    if (returnValue == 0) {
        return InvokeResult::success();
    } else {
        return InvokeResult::failure(file.read().toLocal8Bit());
    }
}

}
}
