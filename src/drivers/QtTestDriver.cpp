#include "cucumber-cpp/internal/drivers/QtTestDriver.hpp"
#include "cucumber-cpp/internal/utils/qtCapture.hpp"

#include <QtTest/QtTest>

namespace cucumber {
namespace internal {

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

