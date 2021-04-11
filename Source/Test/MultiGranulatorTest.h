/*
  ==============================================================================

    MultiGranulatorTest.h
    Created: 10 Apr 2021 10:21:25pm
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include "../Granulator/MultiGranulator.h"
#include <JuceHeader.h>

class MultiGranulatorTest : public juce::UnitTest {
public:
  MultiGranulatorTest()
      : juce::UnitTest("MultiGranulator Test", "QuickSand"),
        gran{&gran_settings} {};

  void runTest() override {
    beginTest("Fill MultiGranulator");
  }

private:
  GranulatorSettings gran_settings;
  MultiGranulator gran;
};