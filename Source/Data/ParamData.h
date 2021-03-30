/*
  ==============================================================================

    ParamData.h
    Created: 29 Mar 2021 3:20:42pm
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct param {
  float min;
  float max;
  float step;
  float initial;
  std::string id;
  std::string name;
  std::string suffix;
};

const struct param CACHE_SIZE_PARAM_DATA = {
  500, 5000, 1, 1000,
  "cache_size", "Cache Size", " ms"
};
const struct param GRAIN_SIZE_PARAM_DATA = {
  1, 450, 0.1, 20,
  "grain_size", "Grain Size", " ms"
};
const struct param GRAIN_SHAPE_PARAM_DATA = {
  5, 100, 1, 50,
  "grain_shape", "Shape", "%"
};
const struct param GRAIN_RAND_PARAM_DATA = {
  0, 100, 1, 50,
  "randomness", "Randomness", "%"
};
const struct param GRAIN_OVERLAP_PARAM_DATA = {
  0.5, 2.0, 0.01, 1.0,
  "overlap", "Overlap", " grains"
};
const struct param WET_DRY_PARAM_DATA = {
  0, 100, 0.1, 100,
  "wet_dry", "Wet/Dry", "%"
};