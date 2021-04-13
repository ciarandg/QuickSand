/*
  ==============================================================================

    MultiGranulator.cpp
    Created: 4 Apr 2021 7:01:04pm
    Author:  ciaran

  ==============================================================================
*/

#include "MultiGranulator.h"
#include "GranulatorSettings.h"

MultiGranulator::MultiGranulator(GranulatorSettings *settings,
                                 RollingCache *cache)
    : settings{settings}, cache{cache} {
  for (int g = 0; g < granulators.size(); ++g)
    granulators[g] = {settings, cache};
};

std::vector<float> MultiGranulator::read(int totalSamples) {
  std::vector<float> out;
  out.resize(totalSamples);
  for (int s = 0; s < totalSamples; ++s)
    out[s] = 0;

  int gs = settings->grainSize;
  float ov = settings->overlap;
  float rand = settings->randomness;
  for (int g = 0; g < granulators.size(); ++g) {
    std::vector<float> voice = granulators[g].read(totalSamples, gs, ov, rand);
    if (g < voiceCount) for (int s = 0; s < totalSamples; ++s) {
      out[s] += voice[s];
    }
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
    cache->write(mono[samp]);
  }
}

void MultiGranulator::resize(uint new_size) {
  for (Granulator g : granulators)
    g.clear_overhang();
  cache->resize(new_size);
}

void MultiGranulator::set_voice_count(uint count) {
  voiceCount = count < MAX_GRANULATOR_COUNT ? count : MAX_GRANULATOR_COUNT;
  for (Granulator gran : granulators) {
    gran.clear_overhang();
  }
}