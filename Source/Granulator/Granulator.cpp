/*
  ==============================================================================

    Granulator.cpp
    Created: 29 Mar 2021 11:40:26pm
    Author:  ciaran

  ==============================================================================
*/

#include "Granulator.h"
#include "RollingCache.cpp"

Granulator::Granulator() : ringBuf(0){};

std::vector<float> Granulator::read(int totalSamples) {
  int samplesToFill = totalSamples;
  int samplesFilled = 0;
  std::vector<float> out;

  if (!ringBuf.is_full()) {
    for (; samplesFilled < samplesToFill; ++samplesFilled)
      out.push_back(0.f);
    return out;
  }

  std::vector<float> grain;
  int maxOffset = (ringBuf.get_capacity() - grainSize) * randomness;
  for (; samplesFilled < totalSamples; ++samplesFilled) {
    float samp = 0;
    if (oh.samplesToNextGrain-- <= 0) {
      auto offset =
          maxOffset <= 0
              ? 0
              : random.nextInt(
                    maxOffset); // conditional prevents assertion failure
      grain = ringBuf.read_chunk(grainSize, offset);
      apply_ramp(grain);

      oh.samplesToNextGrain = grainSize * overlap;

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

void Granulator::fill(juce::AudioBuffer<float> &source) {
  std::vector<float> mono;
  for (int samp = 0; samp < source.getNumSamples(); ++samp) {
    mono.push_back(0);
  }

  for (int ch = 0; ch < source.getNumChannels(); ++ch) {
    auto *lineIn = source.getReadPointer(ch);
    for (int samp = 0; samp < source.getNumSamples(); ++samp) {
      mono[samp] += lineIn[samp];
    }
  }

  for (int samp = 0; samp < source.getNumSamples(); ++samp) {
    ringBuf.write(mono[samp]);
  }
}

void Granulator::resize(uint new_size) {
  oh = {};
  ringBuf.resize(new_size);
}

void Granulator::set_grain_size(float size) { grainSize = size; }

void Granulator::set_randomness(float pct) { randomness = pct; }

void Granulator::set_window(float pct) { window = pct; }

void Granulator::set_overlap(float grains) { overlap = grains; }

void Granulator::apply_ramp(std::vector<float> &dest) {
  int size = dest.size();
  int ramp_length = float(size) * window; // truncated
  float inc = 1.0f / float(ramp_length);
  for (int i = 0; i < ramp_length; ++i) {
    float mult = inc * i;
    dest[i] *= mult;
    dest[size - 1 - i] *= mult;
  }
}