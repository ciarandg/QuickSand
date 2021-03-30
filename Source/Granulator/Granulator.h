/*
  ==============================================================================

    Granulator.h
    Created: 29 Mar 2021 11:40:26pm
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include "RollingCache.h"
#include <JuceHeader.h>

class Granulator {
public:
  Granulator();

  // PRE: size() >= numSamples
  // poll sample from ringBuf numSamples times
  // return vector containing random chunks
  std::vector<float> read(int totalSamples);

  // PRE: ringBuf has enough space to contain entirety of the requested channel
  // fill ringBuf with the entirety of the specified channel of specified source
  void fill(juce::AudioBuffer<float> &source);

  void resize(uint new_size);
  void set_grain_size(float size);
  void set_randomness(float pct);
  void set_window(float pct);
  void set_overlap(float grains);

private:
  struct overhang {
    std::deque<float> data;
    int samplesToNextGrain = 0;
  };

  RollingCache ringBuf;
  juce::Random random;
  int grainSize;
  float randomness;
  float window;
  float overlap;
  struct overhang oh;
  
  void apply_ramp(std::vector<float> &dest);
};