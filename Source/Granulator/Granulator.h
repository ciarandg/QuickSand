/*
  ==============================================================================

    Granulator.h
    Created: 29 Mar 2021 11:40:26pm
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include "GranulatorSettings.h"
#include <JuceHeader.h>

class Granulator {
public:
  Granulator();
  Granulator(GranulatorSettings *settings);
  std::vector<float> read(int totalSamples);
  void clear_overhang();

private:
  struct overhang {
    std::deque<float> data;
    int samplesToNextGrain = 0;
  };

  struct overhang oh;
  GranulatorSettings *settings;

  void apply_ramp(std::vector<float> &dest);
};