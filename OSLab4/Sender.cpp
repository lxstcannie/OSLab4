#include <fstream>
#include <iostream>
#include <windows.h>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: Sender.exe <file_name>\n";
        return 1;
    }

    std::string file_name = argv[1];

    HANDLE hInputReadySemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"InputSemaphore");
    HANDLE hOutputReadySemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"OutputSemaphore");
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"FileMutex");

    if (!hInputReadySemaphore || !hOutputReadySemaphore || !hMutex) {
        std::cerr << "Error opening synchronization objects.\n";
        return GetLastError();
    }

    std::cout << "\nInput 1 to write message;\nInput 0 to exit process\n";
    int key;
    std::cin >> key;

    while (key != 0) {
        if (key == 1) {
            WaitForSingleObject(hMutex, INFINITE);

            std::fstream file(file_name, std::ios::out | std::ios::app);
            std::string message;
            std::cout << "Input message to add:\n";
            std::cin >> message;

            file << message << "\n";
            file.close();

            ReleaseMutex(hMutex);
            ReleaseSemaphore(hInputReadySemaphore, 1, NULL);

            WaitForSingleObject(hOutputReadySemaphore, INFINITE);
        }
        else {
            std::cerr << "Invalid input.\n";
        }

        std::cout << "\nInput 1 to write message;\nInput 0 to exit process\n";
        std::cin >> key;
    }

    return 0;
}