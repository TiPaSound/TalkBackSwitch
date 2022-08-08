/*
  ==============================================================================

    UiHandling.cpp
    Created: 8 Aug 2022 4:52:35pm
    Author:  timo

  ==============================================================================
*/

#include "UiHandling.h"


UiHandling::UiHandling()
    :m_pSocket(0)
{
    Connect();
}

UiHandling::~UiHandling()
{
    Disconnect();

    if (0 != m_pSocket)
    {
        delete m_pSocket;
        m_pSocket = 0;
    }
}

void UiHandling::Connect()
{
    // Connection socket
    m_pSocket = new juce::StreamingSocket();
    if (m_pSocket->connect(IPADDRESS, PORT))
    {
        char* ack = "GET /raw HTTP1.1\n\n";
        m_pSocket->write(ack, strlen(ack));
    }
    startTimer(1000);
}

void UiHandling::Disconnect()
{
    char* command = "IOSYS^Disconnexion UI2MCP\n";
    m_pSocket->write(command, strlen(command));

    char sendui[256];
    sprintf(sendui, "QUIT\n");
    m_pSocket->write(sendui, strlen(sendui));

    m_pSocket->close();
}


bool UiHandling::GetMuted(int channel)
{
    bool ret = false;


    return ret;
}

void UiHandling::SetMuted(int channel)
{
    if (m_pSocket->isConnected())
    {
        char* command = "SETD^i.0.mute^1";
        m_pSocket->write(command, strlen(command));
    }
}

void UiHandling::SetUnmuted(int channel)
{
    if (m_pSocket->isConnected())
    {
        char* command = "SETD^i.0.mute^0";
        m_pSocket->write(command, strlen(command));
    }
}


void UiHandling::WatchdogUi()
{
    char* command = "\0";

    /*  Watchdog  */
    command = "ALIVE\n";
    m_pSocket->write(command, strlen(command));

    char s_Cmd[32] = "\0";
    char ShowsCurrent[256] = "";
    sprintf(s_Cmd, "SNAPSHOTLIST^%s\n", ShowsCurrent);
    m_pSocket->write(s_Cmd, strlen(s_Cmd));

    sprintf(s_Cmd, "CUELIST^%s\n", ShowsCurrent);
    m_pSocket->write(s_Cmd, strlen(s_Cmd));

}


void UiHandling::timerCallback()
{
    WatchdogUi();
};