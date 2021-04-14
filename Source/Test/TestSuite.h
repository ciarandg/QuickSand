/*
  ==============================================================================

    TestSuite.h
    Created: 9 Apr 2021 7:10:10pm
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include "GranulatorTest.h"
#include "MultiGranulatorTest.h"
#include "RollingCacheTest.h"

class TestSuite {
public:
  TestSuite(int samplesPerBlock) : multi_granulator_test(samplesPerBlock) {}

private:
  const RollingCacheTest rolling_cache_test;
  const GranulatorTest granulator_test;
  const MultiGranulatorTest multi_granulator_test;
};