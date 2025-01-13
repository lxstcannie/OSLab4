#include <fstream>
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

int main() {
    std::string file_name;
    int number_of_notes;
    int number_of_senders;

    std::cout << "Input binary file name:\n";
    std::cin >> file_name;
    std::cout << "Input number of notes:\n";
    std::cin >> number_of_notes;

    std::fstream file(file_name, std::ios::out);
    file.close();

    std::cout << "Input number of Sender Processes:\n";
    std::cin >> number_of_senders;

    HANDLE hInputReadySemaphore = CreateSemaphore(NULL, 0, number_of_notes, L"InputSemaphore");
    HANDLE hOutputReadySemaphore = CreateSemaphore(NULL, 0, number_of_notes, L"OutputSemaphore");
    HANDLE hMutex = CreateMutex(NULL, FALSE, L"FileMutex");

    if (!hInputReadySemaphore || !hOutputReadySemaphore || !hMutex) {
        std::cerr << "Error creating synchronization objects.\n";
        return GetLastError();
    }

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;
    std::vector<HANDLE> hEvents(number_of_senders);

    for (int i = 0; i < number_of_senders; ++i) {
        std::string sender_cmd = "Sender.exe " + file_name;
        std::wstring cmd_w(sender_cmd.begin(), sender_cmd.end());

        hEvents[i] = CreateEvent(NULL, FALSE, FALSE, (L"SenderStarted" + std::to_wstring(i)).c_str());

        if (!hEvents[i] ||
            !CreateProcess(NULL, &cmd_w[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            std::cerr << "Error starting sender process.\n";
            return GetLastError();
        }

        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }

    WaitForMultipleObjects(number_of_senders, hEvents.data(), TRUE, INFINITE);

    for (HANDLE event : hEvents) {
        CloseHandle(event);
    }

    // Остальная часть кода без изменений
}