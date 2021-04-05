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

MultiGranulator::MultiGranulator() : ringBuf(0){};

std::vector<float> MultiGranulator::read(int totalSamples) {
  return gran.read(totalSamples);
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
  gran.clear_overhang();
  ringBuf.resize(new_size);
}

void MultiGranulator::set_grain_size(float size) { grainSize = size; }

void MultiGranulator::set_randomness(float pct) { randomness = pct; }

void MultiGranulator::set_window(float pct) { window = pct; }

void MultiGranulator::set_overlap(float grains) { overlap = grains; }
