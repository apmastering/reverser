#include "APCommon.h"
#include "PluginProcessor.h"


Reverser::Reverser()
: AudioProcessor(BusesProperties()
                 .withInput("Input", juce::AudioChannelSet::stereo(), true)
                 .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
apvts(*this, nullptr, "PARAMETERS", createParameterLayout()),
parameterList(static_cast<int>(ParameterNames::END) + 1) {
        
    for (int i = 0; i < static_cast<int>(ParameterNames::END); ++i) {
        
        parameterList[i] = static_cast<juce::AudioParameterFloat*>(apvts.getParameter(queryParameter(static_cast<ParameterNames>(i)).id));
    }
}


void Reverser::prepareToPlay(double sampleRate, int samplesPerBlock) {
}


float Reverser::getFloatKnobValue(ParameterNames parameter) const {
    
    return parameterList[static_cast<int>(parameter)]->get();
}


void Reverser::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    
    juce::ScopedNoDenormals noDenormals;

    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) buffer.clear(i, 0, buffer.getNumSamples());
    
    const float inputGainValue  = decibelsToGain(getFloatKnobValue(ParameterNames::inGain));
    const float outputGainValue = decibelsToGain(getFloatKnobValue(ParameterNames::outGain));
    const bool  reverse         = getFloatKnobValue(ParameterNames::reverse) > 0.5;

    // ReSharper disable once CppTooWideScopeInitStatement
    const size_t clippingCountdownLocal = clippingCountdown.load(std::memory_order_acquire);
    if (clippingCountdownLocal > 0) clippingCountdown.store(clippingCountdownLocal - 1, std::memory_order_release);

    float* channelData[2];
    
    for (int i = 0; i < totalNumInputChannels && i < 2; i++) channelData[i] = buffer.getWritePointer(i);
    
    if (reverse) {
        bool clipped = false;

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            for (int channel = 0; channel < 2 && channel < totalNumInputChannels; channel++) {
                
                float s = channelData[channel][sample];
                s *= inputGainValue;
                s = std::atanh(s);
                s *= 0.3f;
                s *= outputGainValue;

                if (s > 1.2f) {
                    s = 1.2f;
                    clipped = true;
                }
                if (s < -1.2f) {
                    s = -1.2f;
                    clipped = true;
                }

                channelData[channel][sample] = s;
            }
        }

        if (clipped) clippingCountdown.store(clippingCountdownAmount, std::memory_order_release);
        return;
    }
        
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        for (int channel = 0; channel < 2 && channel < totalNumInputChannels; channel++) {
            
            float s = channelData[channel][sample];
            s *= inputGainValue;
            s = std::tanh(s);
            channelData[channel][sample] = s;
        }
    }
}