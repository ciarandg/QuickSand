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

  void runTest() override {
    beginTest("Ensure zeroed output while cache is unfilled");
    for (int i = 0; i < CACHE_SIZE; ++i) {
      std::vector<float> zeroed = gran.read(500);
      expect(zeroed.size() == 500);
      for (float z : zeroed)
        expect(z == 0.f);
      cache.write(i);
    }

    beginTest("Test single grain");
    std::vector<float> grain = gran.read(GRAIN_SIZE);
    expect(grain.size() == GRAIN_SIZE);
    for (int s = 0; s < grain.size(); ++s)
      expect(grain[s] == CACHE_SIZE - GRAIN_SIZE + s);

    beginTest("Test triple grain");
    grain = gran.read(GRAIN_SIZE * 3);
    expect(grain.size() == GRAIN_SIZE * 3);
    for (int s = 0; s < grain.size(); ++s)
      expect(grain[s] == CACHE_SIZE - GRAIN_SIZE + (s % GRAIN_SIZE));

    beginTest("Test partial grain");
    grain = gran.read(GRAIN_SIZE * 2.5);
    expect(grain.size() == GRAIN_SIZE * 2.5);
    for (int s = 0; s < grain.size(); ++s)
      expect(grain[s] == CACHE_SIZE - GRAIN_SIZE + (s % GRAIN_SIZE));

    beginTest("Test overlap");
    // ? what if grain size is larger than cache size
  }

private:
  const uint CACHE_SIZE = 40;
  const int GRAIN_SIZE = 5;
  const float GRAIN_SHAPE = 0.0; // square
  const float RANDOMNESS = 0.0;
  const float OVERLAP = 1.0; // back-to-back

  GranulatorSettings settings{GRAIN_SIZE, GRAIN_SHAPE, RANDOMNESS, OVERLAP};
  RollingCache cache{CACHE_SIZE};
  Granulator gran{&settings, &cache};
};