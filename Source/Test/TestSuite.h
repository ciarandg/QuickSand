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
private:
  const double SAMP_RATE = 48000;
  const int SAMPS_PER_BLOCK = 512;

public:
  TestSuite() {}

  RollingCacheTest rolling_cache_test;
  GranulatorTest granulator_test{SAMP_RATE, SAMPS_PER_BLOCK};
  MultiGranulatorTest multi_granulator_test{SAMP_RATE, SAMPS_PER_BLOCK};
};