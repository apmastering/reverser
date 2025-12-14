#pragma once

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>


class KnobLook1 final : public juce::LookAndFeel_V4 {
public:
    KnobLook1();
    void drawRotarySlider (juce::Graphics& g, 
                           int x,
                           int y,
                           int width,
                           int height,
                           float sliderPosProportional,
                           float rotaryStartAngle,
                           float rotaryEndAngle, 
                           juce::Slider& slider) override;
private:
    juce::Image knobImage;
};


void drawKnob(juce::Graphics &g,
              const juce::Image &knobImage,
              float rotaryEndAngle,
              float rotaryStartAngle,
              float sliderPosProportional,
              int width,
              int &x,
              int &y);
