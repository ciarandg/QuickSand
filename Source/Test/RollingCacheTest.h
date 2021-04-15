/*
  ==============================================================================

    RollingCacheTest.h
    Created: 9 Apr 2021 5:45:12pm
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include "../Granulator/RollingCache.h"
#include <JuceHeader.h>

class RollingCacheTest : public juce::UnitTest {
public:
  RollingCacheTest() : juce::UnitTest("Rolling Cache Test", "QuickSand"){};

  void runTest() override {
    beginTest("Check initialization");
    expect(cache.get_capacity() == INIT_SIZE);
    expect(!cache.is_full());

    beginTest("Fill cache");
    testFillFromEmpty();

    beginTest("Read entire buffer");
    std::vector<float> chunk;
    chunk.resize(INIT_SIZE);
    cache.read_chunk(INIT_SIZE, 0, chunk);
    for (int s = 0; s < INIT_SIZE; ++s) {
      expect(chunk[s] == (float) s);
    }

    beginTest("Read chunks of random sizes and offsets");
    testRandomChunks();

    beginTest("Overwrite buffer, checking validity each sample");
    int cap = cache.get_capacity();
    for (int w = 0; w < cap; ++w) {
      cache.write(cap + w);
      std::vector<float> buf;
      buf.resize(cap);
      cache.read_chunk(cap, 0, buf);
      for (int r = 0; r < cap; ++r) {
        expect(buf[r] == (r < cap - w) ? r + 1 : cap + w);
      }
    }

    beginTest("Resize and refill");
    cache.resize(OTHER_SIZE);
    expect(cache.get_capacity() == OTHER_SIZE);
    testFillFromEmpty();

    beginTest("Read random chunks (resized cache)");
    testRandomChunks();
  }

private:
  const uint INIT_SIZE = 1211;
  const uint OTHER_SIZE = 893;
  const uint NUM_RANDOM_CHUNK_ITER = 100;
  RollingCache cache{INIT_SIZE};
  
  int randMod(int range) {
    return abs(getRandom().nextInt()) % range;
  }

  void testFillFromEmpty() {
    for (int s = 0; s < cache.get_capacity(); ++s) {
      expect(!cache.is_full());
      cache.write((float)s);
    }
    expect(cache.is_full());
  }
  
  void testRandomChunks() {
    const uint cache_size = cache.get_capacity();

    for (int t = 0; t < NUM_RANDOM_CHUNK_ITER; ++t) {
      expect(cache.is_full());
      const uint chunk_size = randMod(cache_size);
      const uint offset = randMod(cache_size - chunk_size + 1);
      std::vector<float> chunk;
      chunk.resize(chunk_size);
      cache.read_chunk(chunk_size, offset, chunk);
      
      for (int s = 0; s < chunk_size; ++s) {
        expect(chunk[s] == (float) (cache_size - chunk_size - offset + s));
      }
      expect(cache.is_full());
    }
  }
};