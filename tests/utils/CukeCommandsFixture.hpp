#ifndef CUKE_CUKECOMMANDSFIXTURE_HPP_ 
#define CUKE_CUKECOMMANDSFIXTURE_HPP_

#include <cucumber-cpp/internal/CukeCommands.hpp>
#include <cucumber-cpp/internal/drivers/GenericDriver.hpp>
#include "StepManagerTestDouble.hpp"

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

using namespace cucumber::internal;
using boost::shared_ptr;

class EmptyStep : public GenericStep {
    void body() {}
};

class CukeCommandsFixture : public ::testing::Test, public CukeCommands {
    StepManagerTestDouble stepManager;
public:
    const static std::string STATIC_MATCHER;

protected:
    shared_ptr<StepInfo> stepInfoPtr;

    template<class T>
    void runStepBodyTest() {
        addStepToManager<T>(STATIC_MATCHER);
        shared_ptr<const InvokeArgs> spArgs(T::buildInvokeArgs());
        invoke(stepInfoPtr->id, spArgs.get());
    }

    template<class T>
    void addStepToManager(const std::string &matcher) {
        stepInfoPtr = boost::make_shared< StepInvoker<T> >(matcher, "");
        stepManager.addStep(stepInfoPtr);
    }

    virtual void TearDown() {
        stepManager.clearSteps();
    }
};

const std::string CukeCommandsFixture::STATIC_MATCHER("MATCHER");

#endif /* CUKE_CUKECOMMANDSFIXTURE_HPP_ */

