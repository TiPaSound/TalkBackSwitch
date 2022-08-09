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
    m_pSocket(0),
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

    if (0 != m_pSocket)
    {
        delete m_pSocket;
        m_pSocket = 0;
    }
}

bool UiHandling::Connect()
{
    bool rv = false;
    // Connection socket
    m_pSocket = new juce::StreamingSocket();
    rv = m_pSocket->connect(IPADDRESS, PORT);
    if (rv)
    {
        char* ack = "GET /raw HTTP1.1\n\n";
        m_pSocket->write(ack, (int)strlen(ack));
    }
    return rv;

}

void UiHandling::Disconnect()
{
    char* command = "IOSYS^Disconnexion UI2MCP\n";
    m_pSocket->write(command, (int)strlen(command));

    char sendui[256];
    sprintf(sendui, "QUIT\n");
    m_pSocket->write(sendui, (int)strlen(sendui));

    m_pSocket->close();
}


bool UiHandling::GetMuted(int /*channel*/)
{
    return m_muted;
}

void UiHandling::SetMuted(int /*channel*/)
{
    if (m_pSocket->isConnected())
    {
        char* command = "SETD^i.0.mute^1";
        m_pSocket->write(command, (int)strlen(command));
    }
}

void UiHandling::SetUnmuted(int /*channel */)
{
    if (m_pSocket->isConnected())
    {
        char* command = "SETD^i.0.mute^0";
        m_pSocket->write(command, (int)strlen(command));
    }
}


void UiHandling::WatchdogUi()
{
    char* command = "\0";

    m_nWatchdog = juce::Time::currentTimeMillis();
    if (m_nWatchdog >= (m_nWatchdogInit + 1000))
    {
        /*  Watchdog  */
        command = "ALIVE\n";
        m_pSocket->write(command, (int)strlen(command));

        char s_Cmd[32] = "\0";
        char ShowsCurrent[256] = "";
        sprintf(s_Cmd, "SNAPSHOTLIST^%s\n", ShowsCurrent);
        m_pSocket->write(s_Cmd, (int)strlen(s_Cmd));

        sprintf(s_Cmd, "CUELIST^%s\n", ShowsCurrent);
        m_pSocket->write(s_Cmd, (int)strlen(s_Cmd));


        m_nWatchdogInit = juce::Time::currentTimeMillis();
    }
}

void UiHandling::ReadMuted()
{
    m_muted = !m_muted;
}


void UiHandling::run()
{

    do
    {
        //watchdog of ui
        WatchdogUi();

        //read UI Data
        ReadMuted();

        sleep(100);

    } while (!m_stopThread);

};