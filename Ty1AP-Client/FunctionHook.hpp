#include <iostream>
#include <windows.h>

typedef void (*OriginalFunc)();
OriginalFunc originalFunction = nullptr;  // Pointer to store original function

void HookFunction(void* target, void* hook) {
    DWORD oldProtect;
    BYTE jmp[5];

    uintptr_t hookAddress = (uintptr_t)hook;
    uintptr_t targetAddress = (uintptr_t)target;
    intptr_t offset = hookAddress - targetAddress - 5;

    // Backup original bytes
    static BYTE originalBytes[5];
    memcpy(originalBytes, target, 5);

    // Write JMP instruction
    jmp[0] = 0xE9;  // JMP opcode
    memcpy(jmp + 1, &offset, 4);  // Store jump offset

    // Change memory protection to allow writing
    VirtualProtect(target, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(target, jmp, 5);  // Overwrite function prologue
    VirtualProtect(target, 5, oldProtect, &oldProtect);

    // Create a trampoline function
    originalFunction = (OriginalFunc)VirtualAlloc(NULL, 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(originalFunction, originalBytes, 5);  // Restore original bytes
    *((BYTE*)originalFunction + 5) = 0xE9;  // JMP instruction
    *(intptr_t*)((BYTE*)originalFunction + 6) = (targetAddress + 5) - ((uintptr_t)originalFunction + 10);
}