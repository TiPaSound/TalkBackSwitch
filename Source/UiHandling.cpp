/*
  ==============================================================================

    UiHandling.cpp
    Created: 8 Aug 2022 4:52:35pm
    Author:  timo

  ==============================================================================
*/

#include "UiHandling.h"


UiHandling::UiHandling()
    :Thread("uiThread"),
    m_pSocketReceiver(0),
    m_pSocketSender(0),
    m_muted(false),
    m_stopThread(false)
{
    Connect();

    this->startThread();
}

UiHandling::~UiHandling()
{
    m_stopThread = true;
    sleep(500);

    Disconnect();

}

bool UiHandling::Connect()
{
    bool rv = false;
    // Connection socket
    
    m_pSocketSender = new juce::StreamingSocket();
    rv = m_pSocketSender->connect(IPADDRESS, PORT);

    m_pSocketReceiver = new juce::StreamingSocket(); 
    rv = m_pSocketReceiver->connect(IPADDRESS, PORT);
    if (rv)
    {
        sleep(200);
        char* ack = "GET /raw HTTP1.1\n\n";
        m_pSocketReceiver->write(ack, (int)strlen(ack));
        m_pSocketSender->write(ack, (int)strlen(ack));
    }
    return rv;

}

void UiHandling::Disconnect()
{
    char* command = "IOSYS^Disconnexion UI2MCP\n";
    m_pSocketReceiver->write(command, (int)strlen(command));

    char sendui[256];
    sprintf(sendui, "QUIT\n");
    m_pSocketReceiver->write(sendui, (int)strlen(sendui));

    m_pSocketReceiver->close();
    m_pSocketSender->close();

    if (0 != m_pSocketReceiver)
    {
        delete m_pSocketReceiver;
        m_pSocketReceiver = 0;
    }

    if (0 != m_pSocketSender)
    {
        delete m_pSocketSender;
        m_pSocketSender = 0;
    }
}


bool UiHandling::GetMuted(int /*channel*/)
{
    return m_muted;
}

void UiHandling::SetMuted(int /*channel*/)
{
    if (m_pSocketSender->isConnected())
    {
        char* command = "SETD^i.0.mute^1\n";
        m_pSocketSender->write(command, (int)strlen(command));
    }
}

void UiHandling::SetUnmuted(int /*channel */)
{
    if (m_pSocketSender->isConnected())
    {
        char* command = "SETD^i.0.mute^0\n";
        m_pSocketSender->write(command, (int)strlen(command));

    }
}


void UiHandling::WatchdogUi()
{
    char* command = "\0";

    m_nWatchdog = juce::Time::currentTimeMillis();
    if (m_nWatchdog >= (m_nWatchdogInit + 200))
    {
        /*  Watchdog  */
        command = "ALIVE\n";
        m_pSocketReceiver->write(command, (int)strlen(command));
        m_pSocketSender->write(command, (int)strlen(command));

        /*char s_Cmd[32] = "\0";
        char ShowsCurrent[256] = "";
        sprintf(s_Cmd, "SNAPSHOTLIST^%s\n", ShowsCurrent);
        m_pSocket->write(s_Cmd, (int)strlen(s_Cmd));

        sprintf(s_Cmd, "CUELIST^%s\n", ShowsCurrent);
        m_pSocket->write(s_Cmd, (int)strlen(s_Cmd));*/

        //char* ack = "GET /raw HTTP1.1\n\n";
        //m_pSocketReceiver->write(ack, (int)strlen(ack));

        m_nWatchdogInit = juce::Time::currentTimeMillis();
    }
}

void UiHandling::ReadMuted()
{
    char buffer[MAX_BUFFER] = "\0";
    int length = m_pSocketReceiver->read(&buffer[0], MAX_BUFFER-1, false); //MAX_BUFFER - 1: last must be 0x0

    juce::String strBuffer = buffer;
    juce::StringArray tokens;
    tokens.addTokens(strBuffer, "\n", "");

    for (int i = 0; i < tokens.size(); i++)
    {
        juce::String s = tokens[i]; // holds next token
        int idx = s.indexOf("SETD^i.0.mute^0");
        if (0 <= idx)
            m_muted = false;

        idx = s.indexOf("SETD^i.0.mute^1");
        if (0 <= idx)
            m_muted = true;
    }

}




void UiHandling::run()
{

    do
    {
        //watchdog of ui
        WatchdogUi();

        //read UI Data
        ReadMuted();

        sleep(30);

    } while (!m_stopThread);

};