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
      : juce::UnitTest("MultiGranulator Test", "QuickSand"), gran{
                                                                 &gran_settings,
                                                                 &cache} {};

  void runTest() override {
    beginTest("Fill MultiGranulator");
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

    beginTest("Test single grain with two voices");
    gran.set_voice_count(2);
    grain = gran.read(GRAIN_SIZE);
    expect(grain.size() == GRAIN_SIZE);
    for (int s = 0; s < grain.size(); ++s)
      expect(grain[s] == 2 * (CACHE_SIZE - GRAIN_SIZE + s));

    beginTest("Test single grain with sixteen voices");
    gran.set_voice_count(16);
    grain = gran.read(GRAIN_SIZE);
    expect(grain.size() == GRAIN_SIZE);
    for (int s = 0; s < grain.size(); ++s)
      expect(grain[s] == 16 * (CACHE_SIZE - GRAIN_SIZE + s));

    beginTest("Check single grain with new grain size");
    gran_settings.grainSize = 9;
    grain = gran.read(gran_settings.grainSize);
    expect(grain.size() == gran_settings.grainSize);
    for (int s = 0; s < grain.size(); ++s)
      expect(grain[s] == 16 * (CACHE_SIZE - gran_settings.grainSize + s));

    beginTest("Cycle writing/reading");
    int samps[CACHE_SIZE];
    for (int s = 0; s < CACHE_SIZE; cache.write(s), samps[s] = s, ++s)
      ;
    for (int s = 0; s < CACHE_SIZE; ++s) {
      int val = 2 * CACHE_SIZE + s;
      cache.write(val);
      samps[s] = val;
      std::vector<float> triple_grain = gran.read(gran_settings.grainSize * 3);
      for (int gs = 0; gs < gran_settings.grainSize * 3; ++gs) {
        expect(triple_grain[gs] == gs % gran_settings.grainSize <
                                       gran_settings.grainSize - s - 1
                   ? gran.voiceCount * (CACHE_SIZE - gran_settings.grainSize +
                                        (gs % gran_settings.grainSize) + 1)
                   : 2 * CACHE_SIZE + gs);
      }
    }
  }

private:
  const uint CACHE_SIZE = 40;
  const int GRAIN_SIZE = 5;
  const float GRAIN_SHAPE = 0.0; // square
  const float RANDOMNESS = 0.0;
  const float OVERLAP = 1.0; // back-to-back

  GranulatorSettings gran_settings{GRAIN_SIZE, GRAIN_SHAPE, RANDOMNESS,
                                   OVERLAP};
  RollingCache cache{CACHE_SIZE};
  MultiGranulator gran;
};