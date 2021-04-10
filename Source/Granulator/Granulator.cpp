/*
  ==============================================================================

    Granulator.cpp
    Created: 29 Mar 2021 11:40:26pm
    Author:  ciaran

  ==============================================================================
*/

#include "Granulator.h"
#include "GranulatorSettings.h"
#include "MultiGranulator.h"
#include "RollingCache.h"

Granulator::Granulator() {}
Granulator::Granulator(GranulatorSettings *settings, RollingCache *cache) : settings{settings}, cache{cache} {}

std::vector<float> Granulator::read(int totalSamples) {
  int samplesToFill = totalSamples;
  int samplesFilled = 0;
  std::vector<float> out;

  if (!cache->is_full()) {
    for (; samplesFilled < samplesToFill; ++samplesFilled)
      out.push_back(0.f);
    return out;
  }

  std::vector<float> grain;
  int maxOffset =
      (cache->get_capacity() - settings->grainSize) * settings->randomness;
  for (; samplesFilled < totalSamples; ++samplesFilled) {
    float samp = 0;
    if (oh.samplesToNextGrain-- <= 0) {
      auto offset =
          maxOffset <= 0
              ? 0
              : random.nextInt(
                    maxOffset); // conditional prevents assertion failure
      grain = cache->read_chunk(settings->grainSize, offset);
      apply_ramp(grain);

      oh.samplesToNextGrain = settings->grainSize * settings->overlap;

      for (int i = 0; i < grain.size(); ++i) {
        if (i < oh.data.size())
          oh.data[i] += grain[i];
        else
          oh.data.push_back(grain[i]);
      }
    }

    if (!oh.data.empty()) {
      samp += oh.data.front();
      oh.data.pop_front();
    }
    out.push_back(samp);
  }

  return out;
}

void Granulator::clear_overhang() { oh = {}; }

void Granulator::apply_ramp(std::vector<float> &dest) {
  MultiGranulator *parent = MultiGranulator::Instance();
  int size = dest.size();
  int ramp_length = float(size) * settings->grainShape; // truncated
  float inc = 1.0f / float(ramp_length);
  for (int i = 0; i < ramp_length; ++i) {
    float mult = inc * i;
    dest[i] *= mult;
    dest[size - 1 - i] *= mult;
  }
}