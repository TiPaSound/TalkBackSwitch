/*
  ==============================================================================

    UiHandling.h
    Created: 8 Aug 2022 4:52:35pm
    Author:  timo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class UiHandling : public juce::Timer
{
public: 
    UiHandling();
    ~UiHandling();

    bool GetMuted(int channel);
    void SetMuted(int channel);
    void SetUnmuted(int channel);

private:
#define IPADDRESS   "10.10.2.2"
#define PORT        80

    void Connect();
    void Disconnect();
    void WatchdogUi();
    void timerCallback() override;

    juce::StreamingSocket* m_pSocket;

};