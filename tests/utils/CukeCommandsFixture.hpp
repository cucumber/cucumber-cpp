#ifndef CUKE_CUKECOMMANDSFIXTURE_HPP_
#define CUKE_CUKECOMMANDSFIXTURE_HPP_

#include <cucumber-cpp/internal/CukeCommands.hpp>
#include <cucumber-cpp/internal/drivers/GenericDriver.hpp>
#include "StepManagerTestDouble.hpp"

#include <gtest/gtest.h>
#include <memory>

using namespace cucumber::internal;

class EmptyStep : public GenericStep {
    void body() override {
    }
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
    void addStepToManager(const std::string& matcher) {
        stepId = StepManager::addStep(std::make_shared<StepInvoker<T>>(matcher, ""));
    }

    void TearDown() override {
        StepManager::clearSteps();
    }
};

const std::string CukeCommandsFixture::STATIC_MATCHER("MATCHER");

#endif /* CUKE_CUKECOMMANDSFIXTURE_HPP_ */
