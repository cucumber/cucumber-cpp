#ifndef CUKE_CUKECOMMANDSFIXTURE_HPP_ 
#define CUKE_CUKECOMMANDSFIXTURE_HPP_

#include <cucumber-cpp/internal/CukeCommands.hpp>
#include <cucumber-cpp/internal/drivers/GenericDriver.hpp>
#include "StepManagerTestDouble.hpp"

#include <boost/make_shared.hpp>

using namespace cucumber::internal;
using boost::shared_ptr;

class EmptyStep : public GenericStep {
    void body() {}
};

class CukeCommandsFixture : public ::testing::Test, public CukeCommands {
    typedef StepManagerTestDouble StepManager;

public:
    const static std::string STATIC_MATCHER;

protected:
    step_id_type stepId;

    template<class T>
    void runStepBodyTest() {
        addStepToManager<T>(STATIC_MATCHER);
        const InvokeArgs spArgs(T::buildInvokeArgs());
        invoke(stepId, &spArgs);
    }

    template<class T>
    void addStepToManager(const std::string &matcher) {
        stepId = StepManager::addStep(boost::make_shared<StepInvoker<T> >(matcher, ""));
    }

    virtual void TearDown() {
        StepManager::clearSteps();
    }
};

const std::string CukeCommandsFixture::STATIC_MATCHER("MATCHER");

#endif /* CUKE_CUKECOMMANDSFIXTURE_HPP_ */

