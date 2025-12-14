#include "APCommon.h"
#include "PluginProcessor.h"


APComp::APComp()
: AudioProcessor(BusesProperties()
                 .withInput("Input", juce::AudioChannelSet::quadraphonic(), true)
                 .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
apvts(*this, nullptr, "PARAMETERS", createParameterLayout()),
parameterList(static_cast<int>(ParameterNames::END) + 1) {
        
    for (int i = 0; i < static_cast<int>(ParameterNames::END); ++i) {
        
        parameterList[i] = static_cast<juce::AudioParameterFloat*>(apvts.getParameter(queryParameter(static_cast<ParameterNames>(i)).id));
    }
}


void APComp::prepareToPlay(double sampleRate, int samplesPerBlock) {
}


float APComp::getFloatKnobValue(ParameterNames parameter) const {
    
    return parameterList[static_cast<int>(parameter)]->get();
}


void APComp::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    
    juce::ScopedNoDenormals noDenormals;

    int totalNumInputChannels = getTotalNumInputChannels();
    int totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) buffer.clear(i, 0, buffer.getNumSamples());
    
    const float inputGainValue  = getFloatKnobValue(ParameterNames::inGain);
    const float outputGainValue = getFloatKnobValue(ParameterNames::outGain);
    const bool  reverse         = getFloatKnobValue(ParameterNames::reverse) > 0.5 ? true : false;
  
    float* channelData[2];
    
    for (int i = 0; i < totalNumInputChannels && i < 2; i++) channelData[i] = buffer.getWritePointer(i);
    
    if (reverse) {
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            for (int channel = 0; channel < 2 && channel < totalNumInputChannels; channel++) {
                
                float s = channelData[channel][sample];
                s = std::atanh(s);
                
                if (s > 1.0f) s = 1.0f;
                if (s < -1.0f) s = -1.0f;
                channelData[channel][sample] *= decibelsToGain(s * outputGainValue);
            }
        }
        
        return;
    }
        
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        for (int channel = 0; channel < 2 && channel < totalNumInputChannels; channel++) {
            
            float s = channelData[channel][sample];
            s *= decibelsToGain(inputGainValue);
            channelData[channel][sample] = std::tanh(s);
        }
    }
}
