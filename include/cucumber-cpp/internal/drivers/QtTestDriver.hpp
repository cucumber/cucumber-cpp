#ifndef CUKE_QTTESTDRIVER_HPP_
#define CUKE_QTTESTDRIVER_HPP_

#include "../step/StepManager.hpp"
#include <cucumber-cpp/internal/CukeExport.hpp>
#include <QObject>

namespace cucumber {
namespace internal {

class CUCUMBER_CPP_EXPORT QtTestStep : public BasicStep {
    friend class QtTestObject;

public:
    QtTestStep() : BasicStep() {}

protected:
    const InvokeResult invokeStepBody();
};

#define STEP_INHERITANCE(step_name) ::cucumber::internal::QtTestStep

class QtTestObject : public QObject {
    Q_OBJECT
public:
    QtTestObject(QtTestStep* qtTestStep) : step(qtTestStep) {}
    virtual ~QtTestObject() {}

protected:
    QtTestStep* step;

private slots:
    void test() const {
        step->body();
    }
};

}
}

#endif /* CUKE_QTTESTDRIVER_HPP_ */
