#pragma once

#include <vector>
#include <atomic>

#include <juce_dsp/juce_dsp.h>
#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "APCommon.h"


class Reverser final : public juce::AudioProcessor {
    
public:
    
    Reverser();
        
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    float getFloatKnobValue(ParameterNames parameter) const;
    juce::AudioProcessorValueTreeState apvts;
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    bool isClipping() const {
        if (clippingCountdown.load(std::memory_order_acquire) > 0) return true;
        return false;
    }
    
private:
    std::atomic <size_t> clippingCountdown;
    static constexpr size_t clippingCountdownAmount = 100;
    std::vector<juce::AudioParameterFloat*> parameterList;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Reverser)
};
