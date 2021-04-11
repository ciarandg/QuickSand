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
    testFill();
    testSingleGrain();
    testTripleGrain();
    testPartialGrain();
    testOverlapMin();
    testOverlapMax();
    testClearOverhang();

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

  void testFill() {
    beginTest("Ensure zeroed output while cache is being filled");
    for (int i = 0; i < CACHE_SIZE; ++i) {
      std::vector<float> zeroed = gran.read(500);
      expect(zeroed.size() == 500);
      for (float z : zeroed)
        expect(z == 0.f);
      cache.write(i);
    }
  }

  void testClearOverhang() {
    beginTest("Test clear_overhang()");
    settings.grainSize = GRAIN_SIZE;
    std::vector<float> grain;

    grain = gran.read(1);
    expect(grain[0] == CACHE_SIZE - GRAIN_SIZE);
    grain = gran.read(1);
    expect(grain[0] == CACHE_SIZE - GRAIN_SIZE + 1);
    gran.clear_overhang();
    grain = gran.read(1);
    expect(grain[0] == CACHE_SIZE - GRAIN_SIZE);
  };

  void testSingleGrain() {
    beginTest("Test single grain");
    std::vector<float> grain = gran.read(GRAIN_SIZE);
    expect(grain.size() == GRAIN_SIZE);
    for (int s = 0; s < grain.size(); ++s)
      expect(grain[s] == CACHE_SIZE - GRAIN_SIZE + s);
  }

  void testTripleGrain() {
    beginTest("Test triple grain");
    std::vector<float> grain = gran.read(GRAIN_SIZE * 3);
    expect(grain.size() == GRAIN_SIZE * 3);
    for (int s = 0; s < grain.size(); ++s)
      expect(grain[s] == CACHE_SIZE - GRAIN_SIZE + (s % GRAIN_SIZE));
  }

  void testPartialGrain() {
    beginTest("Test partial grain");
    int partial = GRAIN_SIZE * 2.5;
    std::vector<float> grain = gran.read(partial);
    // grain: {35, 36, 37, 38, 39, 35, 36, 37, 38, 39, 35, 36}
    expect(grain.size() == partial);
    for (int s = 0; s < grain.size(); ++s)
      expect(grain[s] == CACHE_SIZE - GRAIN_SIZE + (s % GRAIN_SIZE));
  }

  void testOverlapMin() {
    beginTest("Test overlap 0.5");
    gran.clear_overhang();
    int even_grain_size = GRAIN_SIZE % 2 == 0 ? GRAIN_SIZE : GRAIN_SIZE + 1;
    settings.grainSize = even_grain_size;
    settings.overlap = 0.5f;
    int partial = even_grain_size * 1.5;
    std::vector<float> grain = gran.read(partial);

    // grain: 34, 35, 36, 37, 38, 39,
    //        +           34, 35, 36, 37, 38, 39,
    //        +                       34, 35, 36
    //
    //        = {34, 35, 36, 71, 73, 75, 71, 73, 75}

    expect(grain.size() == partial);
    for (int s = 0; s < grain.size(); ++s) {
      int g1 = s < even_grain_size
                   ? CACHE_SIZE - even_grain_size + (s % even_grain_size)
                   : 0;
      int g2 = s >= even_grain_size * 0.5
                   ? CACHE_SIZE - even_grain_size +
                         (int(s + 0.5 * even_grain_size) % even_grain_size)
                   : 0;
      int g3 = s >= even_grain_size
                   ? CACHE_SIZE - even_grain_size + (s % even_grain_size)
                   : 0;
      expect(grain[s] == g1 + g2 + g3);
    }
  }

  void testOverlapMax() {
    beginTest("Test overlap 2.0");
    gran.clear_overhang();
    settings.overlap = 2.0f;
  }

  void testClearOverhang() { beginTest("Test clear_overhang()"); }
};