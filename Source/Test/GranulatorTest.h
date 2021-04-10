/*
  ==============================================================================

    GranulatorTest.h
    Created: 10 Apr 2021 12:27:47pm
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include "../Granulator/Granulator.h"
#include <JuceHeader.h>


class GranulatorTest : public juce::UnitTest {
public:
  GranulatorTest() : juce::UnitTest("Granulator Test", "QuickSand"){};

  void runTest() override { beginTest("Check initialization"); }

private:
  GranulatorSettings settings;
  Granulator gran { &settings };
};