#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    :m_muted(MUTE_NONE)
{
    m_btnMute.onClick = [this] {OnMuteButton(); };
    addAndMakeVisible(m_btnMute);

    setSize (100, 100);
    startTimer(200);
   
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    int margin = 5;
    m_btnMute.setBounds(margin, margin, getWidth() - (2*margin), getHeight() - (2*margin));
}

void MainComponent::OnMuteButton()
{
    SwitchMuteOnUi();
}

void MainComponent::SetMuteStateOnButton(MuteState ms)
{
    if (UNMUTED == ms)
    {
        m_btnMute.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgreen);
    }
    else
    {
        m_btnMute.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkred);
    }
}

void MainComponent::SwitchMuteOnUi()
{
    
    if (MUTED == m_muted)
    {
        // set unmute
        m_ui.SetUnmuted(1);
    }
    else
    {
        //set mute
        m_ui.SetMuted(1);
    }
}

void MainComponent::timerCallback()
{
    bool muted = m_ui.GetMuted(1);
    MuteState ms = MUTE_NONE;
    if (muted)
        ms = MUTED;
    else
        ms = UNMUTED;

    if (ms != m_muted)
    {
        m_muted = ms;
        SetMuteStateOnButton(m_muted);
    }
}
