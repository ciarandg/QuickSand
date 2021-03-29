/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include "PluginProcessor.h"
#include "GUI/ColourScheme.h"
#include "GUI/LabeledRotary.h"

//==============================================================================
/**
*/
class QuickSandAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    QuickSandAudioProcessorEditor (QuickSandAudioProcessor&);
    ~QuickSandAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    QuickSandAudioProcessor& audioProcessor;
    
    static const int TOP_PAD = 40;
    static const int ROTARY_WIDTH = 140;
    static const int ROTARY_HEIGHT = 140;
    static const int GROUP_PAD = 20;
    
    LabeledRotary cacheSizeRotary {
      500, 5000, 1, 1000,
      "Cache Size", " ms", COLOUR_ACCENT_2
    };
    LabeledRotary grainSizeRotary {
      1, 450, 0.1, 20,
      "Grain Size", " ms", COLOUR_ACCENT_1
    };
    LabeledRotary grainShapeRotary {
      0, 100, 1, 50,
      "Shape", "%", COLOUR_ACCENT_1
    };
    LabeledRotary grainRandomnessRotary {
      0.5, 2.0, 0.01, 1.0,
      "Overlap", " grains", COLOUR_ACCENT_1
    };
    LabeledRotary wetDryRotary {
      0, 100, 0.1, 100,
      "Wet/Dry", "%", COLOUR_ACCENT_5
    };
    
    std::vector<std::vector<LabeledRotary*>> dial_groups = {
      { &cacheSizeRotary },
      { &grainSizeRotary, &grainShapeRotary, &grainRandomnessRotary },
      { &wetDryRotary }
    };

    int getDialX(int dial_index) {
      std::vector<int> group_sizes;
      for (std::vector<LabeledRotary*> group : dial_groups) {
        group_sizes.push_back(group.size());
      }
      
      int x_pos = 0, acc = 0;
      for (int s : group_sizes) {
        int group_end = acc+s;
        
        bool dial_in_group = dial_index < group_end;
        x_pos += ROTARY_WIDTH*(dial_in_group ? dial_index-acc : s);
        if (dial_in_group) break;
        
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QuickSandAudioProcessorEditor)
};
