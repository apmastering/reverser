#pragma once

#include "PluginProcessor.h"
#include "KnobLooks.h"

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>


class GUI final : public juce::AudioProcessorEditor, private juce::Timer {

public:
    
    GUI (APComp&);
    ~GUI() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void mouseDown (const juce::MouseEvent& event) override;
    ButtonName determineButton(const juce::MouseEvent &event);
    void reverseMode(bool active);
    
private:
    
    APComp& audioProcessor;
        
    KnobLook1 knobLook1;
        
    juce::Image backgroundImage;
            
    juce::Slider inGainSlider;
    juce::Slider outGainSlider;
    juce::Slider reverseSlider;
            
    std::vector<std::pair<std::string, std::reference_wrapper<juce::Slider>>> sliders {
        {"inGainSlider",        std::ref(inGainSlider)},
        {"outGainSlider",       std::ref(outGainSlider)},
        {"reverseSlider",       std::ref(reverseSlider)},
    };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverseAttachment;
    
    const int row1 = 359;
    const int row2 = 442;
    const int column1 = 125;
    const int column2 = 327;
    const int buttonRadius = 18;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GUI)
};
