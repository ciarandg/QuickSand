/*
  ==============================================================================

    MultiGranulator.h
    Created: 4 Apr 2021 7:01:04pm
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include "Granulator.h"
#include "GranulatorSettings.h"
#include "RollingCache.h"
#include <JuceHeader.h>

class MultiGranulator {
public:
  static MultiGranulator *Instance();

  // PRE: size() >= numSamples
  // poll sample from ringBuf numSamples times
  // return vector containing random chunks
  std::vector<float> read(int totalSamples);

  // PRE: ringBuf has enough space to contain entirety of the requested channel
  // fill ringBuf with the entirety of the specified channel of specified source
  void fill(juce::AudioBuffer<float> &source);

  void resize(uint new_size);
  void set_voice_count(uint count);
  void set_grain_size(float size);
  void set_randomness(float pct);
  void set_window(float pct);
  void set_overlap(float grains);
  RollingCache ringBuf;

  int voiceCount = 1;
  GranulatorSettings settings;
  juce::Random random;

protected:
  MultiGranulator();

private:
  static const int MAX_GRANULATOR_COUNT = 16;
  static MultiGranulator *_instance;
  std::array<Granulator, MAX_GRANULATOR_COUNT> granulators;
};