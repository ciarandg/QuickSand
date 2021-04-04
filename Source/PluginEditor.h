/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "Data/ParamData.h"
#include "PluginProcessor.h"
#include "UI/ColourScheme.h"
#include "UI/EnvelopeGraphic.h"
#include "UI/LabeledRotary.h"
#include <JuceHeader.h>
#include <vector>

//==============================================================================
/**
 */
class QuickSandAudioProcessorEditor : public juce::AudioProcessorEditor,
                                      public juce::Slider::Listener {
public:
  QuickSandAudioProcessorEditor(QuickSandAudioProcessor &);
  ~QuickSandAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;
  void sliderValueChanged(juce::Slider *slider) override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  QuickSandAudioProcessor &audioProcessor;

  static const int TOP_PAD = 40;
  static const int ROTARY_WIDTH = 140;
  static const int ROTARY_HEIGHT = 140;
  static const int GROUP_PAD = 20;
  static const int GROUP_DIVIDER_WIDTH = 6;

  LabeledRotary cacheSizeRotary{CACHE_SIZE_PARAM_DATA, COLOUR_ACCENT_2};
  juce::SliderParameterAttachment cacheSizeParamAttach{
      *audioProcessor.cacheSizeParameter, cacheSizeRotary};
  LabeledRotary voicesRotary{VOICES_PARAM_DATA, COLOUR_ACCENT_2};
  juce::SliderParameterAttachment voicesParamAttach{
      *audioProcessor.voicesParameter, voicesRotary};
  LabeledRotary grainSizeRotary{GRAIN_SIZE_PARAM_DATA, COLOUR_ACCENT_1};
  juce::SliderParameterAttachment grainSizeParamAttach{
      *audioProcessor.grainSizeParameter, grainSizeRotary};
  LabeledRotary grainShapeRotary{GRAIN_SHAPE_PARAM_DATA, COLOUR_ACCENT_1};
  juce::SliderParameterAttachment grainShapeParamAttach{
      *audioProcessor.grainShapeParameter, grainShapeRotary};
  LabeledRotary grainRandomnessRotary{GRAIN_RAND_PARAM_DATA, COLOUR_ACCENT_1};
  juce::SliderParameterAttachment grainRandomnessParamAttach{
      *audioProcessor.randomnessParameter, grainRandomnessRotary};
  LabeledRotary grainOverlapRotary{GRAIN_OVERLAP_PARAM_DATA, COLOUR_ACCENT_1};
  juce::SliderParameterAttachment grainOverlapParamAttach{
      *audioProcessor.grainOverlapParameter, grainOverlapRotary};
  LabeledRotary wetDryRotary{WET_DRY_PARAM_DATA, COLOUR_ACCENT_5};
  juce::SliderParameterAttachment wetDryParamAttach{
      *audioProcessor.wetDryParameter, wetDryRotary};

  std::vector<std::vector<LabeledRotary *>> dial_groups = {
      {&cacheSizeRotary, &voicesRotary},
      {&grainSizeRotary, &grainShapeRotary, &grainRandomnessRotary,
       &grainOverlapRotary},
      {&wetDryRotary}};

  EnvelopeGraphic grain_shape_graphic{getDialX(2) + ROTARY_WIDTH / 2, 75};

  int getDialX(int dial_index) {
    std::vector<int> group_sizes;
    for (std::vector<LabeledRotary *> group : dial_groups) {
      group_sizes.push_back(group.size());
    }

    int x_pos = 0, acc = 0;
    for (int s : group_sizes) {
      int group_end = acc + s;

      bool dial_in_group = dial_index < group_end;
      x_pos += ROTARY_WIDTH * (dial_in_group ? dial_index - acc : s);
      if (dial_in_group)
        break;

      x_pos += GROUP_PAD;
      acc += s;
    }
    return x_pos;
  }

  int getGroupX(int group_index) {
    int dial_count = 0;
    for (int g = 0; g < group_index; ++g) {
      dial_count += dial_groups[g].size();
    }
    return getDialX(dial_count);
  }

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(QuickSandAudioProcessorEditor)
};
