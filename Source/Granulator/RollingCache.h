/*
  ==============================================================================

    RollingCache.h
    Created: 29 Mar 2021 11:40:54pm
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>

class RollingCache {
public:
  RollingCache(uint capacity);

  void write(float value);
  std::vector<float> read_chunk(size_t chunk_size, int offset);
  void resize(uint new_size);
  size_t get_capacity();
  bool is_full();

private:
  std::vector<float> buffer;
  uint capacity = 0;    // capacity in samples
  uint head = 0;        // incremented by inc_head()
  bool filled = false;  // has the cache been filled yet?

  int wrap_index(int i);
  void inc_head();
};