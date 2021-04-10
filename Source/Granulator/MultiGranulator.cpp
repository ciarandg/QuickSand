/*
  ==============================================================================

    MultiGranulator.cpp
    Created: 4 Apr 2021 7:01:04pm
    Author:  ciaran

  ==============================================================================
*/

#include "MultiGranulator.h"

MultiGranulator *MultiGranulator::_instance = NULL;

MultiGranulator *MultiGranulator::Instance() {
  if (_instance == NULL)
    _instance = new MultiGranulator;
  return _instance;
}

MultiGranulator::MultiGranulator() : ringBuf(0){
  for (int g = 0; g < granulators.size(); ++g) granulators[g] = {&settings};
};

std::vector<float> MultiGranulator::read(int totalSamples) {
  std::vector<float> out;
  out.resize(totalSamples);
  for (int s = 0; s < totalSamples; ++s)
    out[s] = 0;

  for (int g = 0; g < voiceCount; ++g) {
    std::vector<float> voice = granulators[g].read(totalSamples);
    for (int s = 0; s < totalSamples; ++s)
      out[s] += voice[s] / voiceCount;
  }

  return out;
}

void MultiGranulator::fill(juce::AudioBuffer<float> &source) {
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

void MultiGranulator::resize(uint new_size) {
  granulators[0].clear_overhang();
  ringBuf.resize(new_size);
}

void MultiGranulator::set_voice_count(uint count) {
  voiceCount = count < MAX_GRANULATOR_COUNT ? count : MAX_GRANULATOR_COUNT;
  for (Granulator gran : granulators) {
    gran.clear_overhang();
  }
}

void MultiGranulator::set_grain_size(float size) { settings.grainSize = size; }

void MultiGranulator::set_randomness(float pct) { settings.randomness = pct; }

void MultiGranulator::set_window(float pct) { settings.grainShape = pct; }

void MultiGranulator::set_overlap(float grains) { settings.overlap = grains; }
