/*
  ==============================================================================

    Granulator.h
    Created: 29 Mar 2021 11:40:26pm
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include "MultiGranulator.h"
#include "RollingCache.h"
#include <JuceHeader.h>

class Granulator {
public:
  Granulator(MultiGranulator &parent);

  std::vector<float> read(int totalSamples);
  //void resize(uint new_size);
  void clear_overhang();
  //void set_grain_size(float size);
  //void set_randomness(float pct);
  //void set_window(float pct);
  //void set_overlap(float grains);

private:
  struct overhang {
    std::deque<float> data;
    int samplesToNextGrain = 0;
  };

  //RollingCache ringBuf;
  juce::Random random;
  //int grainSize;
  //float randomness;
  //float window;
  //float overlap;
  struct overhang oh;

  MultiGranulator multi;

  void apply_ramp(std::vector<float> &dest);
};