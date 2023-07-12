#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winerror.h>
#include <windivert.h>

#define MAXBUF 0xFFFF

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <delay>\n", argv[0]);
        return 1;
    }

    UINT8 delay = atoi(argv[1]); // Convert delay argument to integer

    HANDLE handle;
    DIVERT_ADDRESS addr;
    char packet[MAXBUF];
    UINT packetLen;

    handle = WinDivertOpen("true", WINDIVERT_LAYER_NETWORK, 0, 0);
    if (handle == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open the WinDivert device (%d)\n", GetLastError());
        return 1;
    }

    printf("WinDivert device opened successfully!\n");

    while (TRUE)
    {
        if (!WinDivertRecv(handle, packet, sizeof(packet), &addr, &packetLen))
        {
            printf("Failed to read packet (%d)\n", GetLastError());
            continue;
        }

        // Inject delay
        Sleep(delay);

        if (!WinDivertSend(handle, packet, packetLen, &addr, NULL))
        {
            printf("Failed to send packet (%d)\n", GetLastError());
            continue;
        }
    }

    WinDivertClose(handle);
    return 0;
}
