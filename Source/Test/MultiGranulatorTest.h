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
  MultiGranulatorTest(int samplesPerBlock)
      : juce::UnitTest("MultiGranulator Test", "QuickSand"),
        gran{samplesPerBlock, &gran_settings, &cache}, samplesPerBlock{
                                                           samplesPerBlock} {};

  void runTest() override {
    beginTest("Fill MultiGranulator");
    for (int i = 0; i < CACHE_SIZE; ++i) {
      std::vector<float> zeroed = gran.read();
      for (float z : zeroed)
        expect(z == 0.f);
      cache.write(i);
    }

    beginTest("Read out with one voice");
    std::vector<float> out = gran.read();
    expect(out.size() == samplesPerBlock);
    for (int s = 0; s < out.size(); ++s) {
      expect(out[s] == CACHE_SIZE - GRAIN_SIZE + (s % GRAIN_SIZE));
    }

    beginTest("Two voices");
    gran.set_voice_count(2);
    out = gran.read();
    expect(out.size() == samplesPerBlock);
    for (int s = 0; s < out.size(); ++s)
      expect(out[s] == 2 * (CACHE_SIZE - GRAIN_SIZE + (s % GRAIN_SIZE)));

    beginTest("Sixteen voices");
    gran.set_voice_count(16);
    out = gran.read();
    expect(out.size() == samplesPerBlock);
    for (int s = 0; s < out.size(); ++s)
      expect(out[s] == 16 * (CACHE_SIZE - GRAIN_SIZE + (s % GRAIN_SIZE)));

    beginTest("Sixteen voices, new grain size");
    gran_settings.grainSize = 9;
    gran.set_voice_count(16);
    out = gran.read();
    expect(out.size() == samplesPerBlock);
    for (int s = 0; s < out.size(); ++s)
      expect(out[s] == 16 * (CACHE_SIZE - gran_settings.grainSize +
                             (s % gran_settings.grainSize)));

    beginTest("Cycle writing/reading");
    int samps[CACHE_SIZE];
    for (int s = 0; s < CACHE_SIZE; cache.write(s), samps[s] = s, ++s)
      ;
    for (int s = 0; s < CACHE_SIZE; ++s) {
      int val = 2 * CACHE_SIZE + s;
      cache.write(val);
      samps[s] = val;
      out = gran.read();
      for (int gs = 0; gs < gran_settings.grainSize * 3; ++gs) {
        expect(out[gs] == gs % gran_settings.grainSize <
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
  int samplesPerBlock;
};