/*
  ==============================================================================

    TestSuite.h
    Created: 9 Apr 2021 7:10:10pm
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include "RollingCacheTest.h"
#include "GranulatorTest.h"

class TestSuite {
private:
  const RollingCacheTest rolling_cache_test{};
  const GranulatorTest granulator_test{};
};