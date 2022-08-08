#pragma once

#include <JuceHeader.h>
#include "UiHandling.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

#define MUTE_CHANNEL    1


class MainComponent  : public juce::Component
{
public:

    enum MuteState
    {
        MuteState_Muted = 1,
        MuteState_Unmuted
    };
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    juce::TextButton m_btnMute{"Mute"};;
    MuteState m_muteState;
    UiHandling m_ui;

    void OnMuteButton();
    void SetMuteStateOnButton();
    void SwitchMuteOnUi();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
