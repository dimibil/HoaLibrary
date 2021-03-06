/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PLUGINEDITOR_H_3EF9D3DB__
#define __PLUGINEDITOR_H_3EF9D3DB__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class HoaSpaceAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    HoaSpaceAudioProcessorEditor (HoaSpaceAudioProcessor* ownerFilter);
    ~HoaSpaceAudioProcessorEditor();

    //==============================================================================
    // This is just a standard Juce paint method...
    void paint (Graphics& g);
};


#endif  // __PLUGINEDITOR_H_3EF9D3DB__
