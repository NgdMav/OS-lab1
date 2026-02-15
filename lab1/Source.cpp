#include <iostream>
#include <fstream>
#include <windows.h>
#include <iomanip>
#include <string>

#include "employee.h"

static HANDLE StartProcess(const char* commandLine)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFOA));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    char cmd[512];
    strcpy_s(cmd, commandLine);

    if (!CreateProcessA(
        NULL,
        cmd,
        NULL,
        NULL,
        FALSE,
        0,          //CREATE_NEW_CONSOLE
        NULL,
        NULL,
        &si,
        &pi))
    {
        std::cerr << "Error: CreateProcess failed (" << GetLastError() << ")\n";
        return NULL;
    }

    CloseHandle(pi.hThread);

    return pi.hProcess;
}

static void DisplayBinaryFile(const char* filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Error: cannot open file " << filename << "\n";
        return;
    }

    std::cout << "\n=== File contents " << filename << " ===\n";
    std::cout << std::string(60, '-') << "\n";
    std::cout << std::left << std::setw(10) << "Nomber"
        << std::setw(15) << "Name"
        << std::setw(15) << "Hours" << "\n";
    std::cout << std::string(60, '-') << "\n";

    employee emp;
    int count = 0;

    while (file.read((char*)&emp, sizeof(employee)))
    {
        std::cout << std::left << std::setw(10) << emp.num
            << std::setw(15) << emp.name
            << std::setw(15) << std::fixed << std::setprecision(2) << emp.hours << "\n";
        count++;
    }

    std::cout << std::string(60, '-') << "\n";
    std::cout << "Total records: " << count << "\n\n";

    file.close();
}

static void DisplayReportFile(const char* filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Error: cannot open report file " << filename << "\n";
        return;
    }

    std::cout << "\n=== Contents of the report " << filename << " ===\n";

    std::string line;
    while (getline(file, line))
    {
        std::cout << line << "\n";
    }

    file.close();
    std::cout << "\n";
}

int main()
{
    char binaryFileName[256];
    int recordCount;
    char reportFileName[256];
    double hourlyRate;
    char commandLine[512];

    std::cout << "=== Program Main ===\n\n";

    std::cout << "Enter the name of the binary file: ";
    std::cin >> binaryFileName;

    std::cout << "Enter the number of records: ";
    std::cin >> recordCount;

    if (recordCount <= 0)
    {
        std::cerr << "Error: the number of records must be positive\n";
        return 1;
    }

    std::cout << "\n--- Launching the Creator utility ---\n";
    sprintf_s(commandLine, "Creator.exe %s %d", binaryFileName, recordCount);

    HANDLE hCreator = StartProcess(commandLine);
    if (hCreator == NULL)
    {
        std::cerr << "Error: failed to start Creator\n";
        return 1;
    }

    std::cout << "Waiting for completion Creator...\n";
    WaitForSingleObject(hCreator, INFINITE);
    CloseHandle(hCreator);
    std::cout << "Creator has completed his work..\n";

    DisplayBinaryFile(binaryFileName);

    std::cout << "Enter the report file name: ";
    std::cin >> reportFileName;

    std::cout << "Enter your hourly rate: ";
    std::cin >> hourlyRate;

    if (hourlyRate <= 0)
    {
        std::cerr << "Error: payment must be positive\n";
        return 1;
    }

    std::cout << "\n--- Launching the Reporter utility ---\n";
    sprintf_s(commandLine, "Reporter.exe %s %s %.2f", binaryFileName, reportFileName, hourlyRate);

    HANDLE hReporter = StartProcess(commandLine);
    if (hReporter == NULL)
    {
        std::cerr << "Error: failed to start Reporter\n";
        return 1;
    }

    std::cout << "Waiting for Reporter to complete...\n";
    WaitForSingleObject(hReporter, INFINITE);
    CloseHandle(hReporter);
    std::cout << "Reporter has completed its work.\n";

    DisplayReportFile(reportFileName);

    std::cout << "=== Main program is completed ===\n";
    system("pause");

    return 0;
}