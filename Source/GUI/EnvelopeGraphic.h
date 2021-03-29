/*
  ==============================================================================

    EnvelopeGraphic.h
    Created: 29 Mar 2021 12:14:04am
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class EnvelopeGraphic {
public:
  EnvelopeGraphic(int center_x, int top_y)
    : top_x{center_x}, top_y{top_y}
  {};

  void draw(float pct, juce::Graphics& g) {
    const float half_top = SIDE_LENGTH/2.f*(-pct+1.f);
    const float x_1 = top_x-half_top;
    const float x_2 = top_x+half_top;
    
    g.drawLine(x_1, top_y, top_x-SIDE_LENGTH/2, top_y+HEIGHT, LINE_WIDTH);
    g.drawLine(x_2, top_y, top_x+SIDE_LENGTH/2, top_y+HEIGHT, LINE_WIDTH);
    g.drawLine(x_1-LINE_WIDTH/2.f+(SOFTEN_OFFSET*pct), top_y, x_2+LINE_WIDTH/2.f-(SOFTEN_OFFSET*pct), top_y, LINE_WIDTH);
  }

  private:
    const float SIDE_LENGTH = 45;
    const int LINE_WIDTH = 4;
    const float SOFTEN_OFFSET = 0.5; // used to smooth out the top of the triangle
    const float HEIGHT = cos(M_PI*(1.f/6.f)) * SIDE_LENGTH;
    int top_x;
    int top_y;
};