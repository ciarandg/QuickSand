/*
  ==============================================================================

    RollingCache.cpp
    Created: 29 Mar 2021 11:40:54pm
    Author:  ciaran

  ==============================================================================
*/

#include "RollingCache.h"
#include <JuceHeader.h>

RollingCache::RollingCache(uint capacity) : capacity{capacity} {
  buffer.resize(capacity);
};

void RollingCache::write(float value) {
  if (capacity == 0)
    return;
  buffer[head] = value;
  inc_head();
}

std::vector<float> RollingCache::read_chunk(size_t chunk_size, int offset) {
  if (!is_full())
    DBG("CAN'T READ FROM UNFILLED BUFFER");
  std::vector<float> out;
  int start = wrap_index(head - offset - chunk_size);

  int targetIndex = wrap_index(head - offset);
  for (int i = start; i != targetIndex; i = wrap_index(i + 1)) {
    out.push_back(buffer[i]);
  }

  return out;
}

void RollingCache::resize(uint new_size) {
  buffer.resize(new_size);
  for (int samp = 0; samp < new_size; ++samp) {
    buffer[samp] = 0.f;
  }
  capacity = new_size;
  head = 0;
  filled = false;
}

size_t RollingCache::get_capacity() { return capacity; }
bool RollingCache::is_full() { return filled; }

int RollingCache::wrap_index(int i) {
  while (i < 0)
    i += capacity;
  return i % capacity;
}

void RollingCache::inc_head() {
  head = wrap_index(head + 1);
  if (!filled && head == capacity - 1)
    filled = true;
}