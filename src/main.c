#include <Windows.h>
#include <errhandlingapi.h>
#include <processthreadsapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <winbase.h>
#include <winnt.h>
#include <winternl.h>
#include <winuser.h>

extern NTSTATUS NTAPI NtSuspendProcess(HANDLE);
extern NTSTATUS NTAPI NtResumeProcess(HANDLE);


LPSTR GetErrorMessage(INT error){
    
    LPSTR string;

    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
    NULL, 
    error, 
    MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), 
    (LPSTR)&string, 
    0, NULL);

    return string;
}

INT main(INT argc, CHAR* argv[]){
    printf("ZivGates Suspender\n" 
                   "Copyright zvqle, All Rights Reserved\n\n");
    if(argc < 2){
        printf("usage: suspend [pid]\n");
        return 1;
    }
    INT pid = atoi(argv[1]);
    if(!pid){
        printf("pid must be an integer\n");
        return 2;
    }

    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS , FALSE, pid);
    if(process == NULL){
        LPSTR message = GetErrorMessage(GetLastError());
        printf("%s", message);
        LocalFree(message);
        return 3;
    }
    NTSTATUS status = NtSuspendProcess(process);
    if(!NT_SUCCESS(status)){
        printf("NtSuspendProcess failed with NTSTATUS 0x%lx", status);
        return 4;
    }
    printf("[!] Press W to unsuspend the process with PID %d\n", pid);
    while(1){
        Sleep(10);
        if(GetAsyncKeyState('W') & 0x8000){
            NTSTATUS status = NtResumeProcess(process);
            if(!NT_SUCCESS(status)){
                printf("NtResumeProcess failed with NTSTATUS 0x%lx", status);
                return 5;
            }
            break;
        }
    }
}