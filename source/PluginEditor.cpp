#include <algorithm>
#include <mutex>

#include "BinaryData.h"

#include "APCommon.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"


GUI::GUI (APComp& p)
: AudioProcessorEditor (&p),
audioProcessor (p),
knobLook1(),
backgroundImage (juce::ImageFileFormat::loadFrom(BinaryData::bg_png, BinaryData::bg_pngSize)),
inGainSlider(),
outGainSlider(),
reverseSlider(),
inGainAttachment (std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "inGain", inGainSlider)),
outGainAttachment (std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "outGain", outGainSlider)),
reverseAttachment (std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "reverse", reverseSlider)) {
          
    for (size_t i = 0; i < sliders.size(); ++i) {
        
        juce::Slider& slider = sliders[i].second.get();
        const std::string& sliderName = sliders[i].first;

        slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        slider.setName(sliderName);
        addAndMakeVisible(slider);
    }
                
    inGainSlider.setLookAndFeel(&knobLook1);
    outGainSlider.setLookAndFeel(&knobLook1);
    
    reverseSlider.setVisible(false);

    setSize (460, 490);
    
    const int refreshRate = 33;
    startTimer(refreshRate);
}


GUI::~GUI() {
    
    stopTimer();
    
    inGainSlider.setLookAndFeel(nullptr);
    outGainSlider.setLookAndFeel(nullptr);
}


void GUI::paint (juce::Graphics& g) {
    
    if (backgroundImage.isValid()) {
        g.drawImage(backgroundImage, getLocalBounds().toFloat());
    } else {
        g.fillAll(juce::Colours::lightgrey);
        g.setColour (juce::Colours::black);
        g.setFont (24.0f);
        g.drawFittedText ("AP Mastering - Distortion Reverser: GUI error", getLocalBounds(), juce::Justification::centredTop, 1);
    }
    
    const bool reverseMode = audioProcessor.getFloatKnobValue(ParameterNames::reverse) > 0.5 ? true : false;
    
    const float buttonFillAmount = 1;
    
    g.setColour(juce::Colours::white.withAlpha(0.4f));
    
    if (reverseMode) {
        g.fillEllipse(column2 - buttonRadius * buttonFillAmount, 
                      row1 - buttonRadius * buttonFillAmount, 
                      buttonRadius * 2 * buttonFillAmount,
                      buttonRadius * 2 * buttonFillAmount);
    } else {
        g.fillEllipse(column1 - buttonRadius * buttonFillAmount,
                      row1 - buttonRadius * buttonFillAmount,
                      buttonRadius * 2 * buttonFillAmount,
                      buttonRadius * 2 * buttonFillAmount);
    }
}


void GUI::resized() {
    
    const int IOknobWidth  = 54;
    const int IOknobHeight = IOknobWidth;
    const int IOradius = IOknobWidth / 2;
    
    inGainSlider.setBounds (column1 - IOradius,
                            row2 - IOradius,
                            IOknobWidth,
                            IOknobHeight);
    outGainSlider.setBounds (column2 - IOradius,
                            row2 - IOradius,
                            IOknobWidth,
                            IOknobHeight);
}


void GUI::timerCallback() {
    
    repaint();
}


ButtonName GUI::determineButton(const juce::MouseEvent &event) {
    
    if (event.y > row1 - buttonRadius &&
        event.y < row1 + buttonRadius) {
        
        if (event.x > column1 - buttonRadius &&
            event.x < column1 + buttonRadius) {

            return ButtonName::saturate;
        }
        
        if (event.x > column2 - buttonRadius &&
            event.x < column2 + buttonRadius) {

            return ButtonName::reverse;
        }
    }
    
    return ButtonName::none;
}

void GUI::mouseDown (const juce::MouseEvent& event) {
    
    ButtonName button = determineButton(event);

    switch (button) {
            
        case ButtonName::none:                               return;
        case ButtonName::saturate: { reverseMode(false);     return; }
        case ButtonName::reverse:  { reverseMode(true);      return; }
        
        default: return;
    }
}


void GUI::reverseMode(bool active) {
    
    reverseSlider.setValue(active ? 1 : 0);
}
