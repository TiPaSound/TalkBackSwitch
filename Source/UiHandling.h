/*
  ==============================================================================

    UiHandling.h
    Created: 8 Aug 2022 4:52:35pm
    Author:  timo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class UiHandling : public juce::Thread
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

    bool Connect();
    void Disconnect();
    void WatchdogUi();
    void ReadMuted();
    void run() override;

    juce::StreamingSocket* m_pSocket;
    juce::int64 m_nWatchdog;
    juce::int64 m_nWatchdogInit;
    bool m_muted;
    bool m_stopThread;


};