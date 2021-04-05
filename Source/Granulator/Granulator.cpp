/*
  ==============================================================================

    Granulator.cpp
    Created: 29 Mar 2021 11:40:26pm
    Author:  ciaran

  ==============================================================================
*/

#include "Granulator.h"
#include "RollingCache.cpp"

Granulator::Granulator(MultiGranulator &parent) : multi(parent){};

std::vector<float> Granulator::read(int totalSamples) {
  int samplesToFill = totalSamples;
  int samplesFilled = 0;
  std::vector<float> out;

  if (!multi.ringBuf.is_full()) {
    for (; samplesFilled < samplesToFill; ++samplesFilled)
      out.push_back(0.f);
    return out;
  }

  std::vector<float> grain;
  int maxOffset =
      (multi.ringBuf.get_capacity() - multi.grainSize) * multi.randomness;
  for (; samplesFilled < totalSamples; ++samplesFilled) {
    float samp = 0;
    if (oh.samplesToNextGrain-- <= 0) {
      auto offset =
          maxOffset <= 0
              ? 0
              : random.nextInt(
                    maxOffset); // conditional prevents assertion failure
      grain = multi.ringBuf.read_chunk(multi.grainSize, offset);
      apply_ramp(grain);

      oh.samplesToNextGrain = multi.grainSize * multi.overlap;

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
  int size = dest.size();
  int ramp_length = float(size) * multi.window; // truncated
  float inc = 1.0f / float(ramp_length);
  for (int i = 0; i < ramp_length; ++i) {
    float mult = inc * i;
    dest[i] *= mult;
    dest[size - 1 - i] *= mult;
  }
}