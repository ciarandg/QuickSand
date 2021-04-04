/*
  ==============================================================================

    Mixer.h
    Created: 29 Mar 2021 11:40:32pm
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Mixer {
public:
  std::vector<float> mix(std::vector<float> dry, std::vector<float> wet,
                         float pct_wet) {
    // assert(dry.size() == wet.size());
    auto size = dry.size();
    float pct_dry = 1.f - pct_wet;
    std::vector<float> out;

    out.resize(size);
    for (int samp = 0; samp < size; ++samp) {
      out[samp] = (wet[samp] * pct_wet) + (dry[samp] * pct_dry);
    }

    return out;
  }
};