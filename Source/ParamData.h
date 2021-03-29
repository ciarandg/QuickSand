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