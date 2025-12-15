#include "PluginProcessor.h"
#include "PluginEditor.h"


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new Reverser(); }
const juce::String Reverser::getName() const { return JucePlugin_Name; }
bool Reverser::acceptsMidi() const { return false; }
bool Reverser::producesMidi() const { return false; }
bool Reverser::isMidiEffect() const { return false; }
double Reverser::getTailLengthSeconds() const { return 0.0; }
int Reverser::getNumPrograms() { return 1; }
int Reverser::getCurrentProgram() { return 0; }
void Reverser::setCurrentProgram (int index) { }
const juce::String Reverser::getProgramName (int index) { return {}; }
void Reverser::changeProgramName (int index, const juce::String& newName) {}
bool Reverser::hasEditor() const { return true; }
void Reverser::releaseResources() {}

bool Reverser::isBusesLayoutSupported (const BusesLayout& layouts) const {

    const auto in  = layouts.getMainInputChannelSet();

    if (const auto out = layouts.getMainOutputChannelSet(); in != out) return false;
    if (in != juce::AudioChannelSet::mono()
     && in != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.inputBuses.size() > 1) {
        return false;
    }
    
    return true;
}

juce::AudioProcessorEditor* Reverser::createEditor() { return new GUI (*this); }

void Reverser::getStateInformation (juce::MemoryBlock& destData) {

    const std::unique_ptr<juce::XmlElement> xml (apvts.state.createXml());
    copyXmlToBinary (*xml, destData);
}

void Reverser::setStateInformation (const void* data, const int sizeInBytes) {
    if (const std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes)); xml != nullptr)
        {
            if (xml->hasTagName (apvts.state.getType()))
            {
                apvts.state = juce::ValueTree::fromXml (*xml);
            }
        }
}
