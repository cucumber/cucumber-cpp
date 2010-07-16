#ifndef CUKEBINS_HPP_
#define CUKEBINS_HPP_

#include "internal/StepManager.hpp"
#include "internal/ContextManager.hpp"
#include "internal/CukeCommands.hpp"

#include "internal/Macros.hpp"

#ifdef GTEST_INCLUDE_GTEST_GTEST_H_
#include "internal/drivers/GTestDriver.hpp"
#else // No test framework
#include "internal/drivers/FakeDriver.hpp"
#endif


#endif /* CUKEBINS_HPP_ */
