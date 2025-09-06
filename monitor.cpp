#include <windows.h>
#include <psapi.h>
#include <iostream>

using namespace std;

void stack() {
    NT_TIB* tib = (NT_TIB*)NtCurrentTeb();
    char local;

    size_t total = (char*)tib->StackBase - (char*)tib->StackLimit;
    size_t usado = (char*)tib->StackBase - &local;

    cout << "Stack total: " << total / 1024 << " KB" << endl;
    cout << "Stack usada: " << usado / 1024 << " KB" << endl;
    cout << "Stack livre: " << (total - usado) / 1024 << " KB" << endl;
}

void heap() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(),
                             (PROCESS_MEMORY_COUNTERS*)&pmc,
                             sizeof(pmc))) {
        cout << "Heap usada: " << pmc.PrivateUsage / 1024 << " KB" << endl;
    } else {
        cout << "Erro ao consultar heap" << endl;
    }
}

void RAM() {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    cout << "RAM usada: " << (statex.ullTotalPhys - statex.ullAvailPhys) / 1024 / 1024 << " MB" << endl;
    cout << "RAM total: " << statex.ullTotalPhys / 1024 / 1024 << " MB" << endl;
}

void monitor() {
    cout << "---------------------" << endl;
    stack();
    heap();
    RAM();
    cout << "---------------------" << endl;
}