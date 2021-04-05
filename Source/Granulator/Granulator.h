/*
  ==============================================================================

    Granulator.h
    Created: 29 Mar 2021 11:40:26pm
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Granulator {
public:
  std::vector<float> read(int totalSamples);
  void clear_overhang();

private:
  struct overhang {
    std::deque<float> data;
    int samplesToNextGrain = 0;
  };

  juce::Random random;
  struct overhang oh;

  void apply_ramp(std::vector<float> &dest);
};