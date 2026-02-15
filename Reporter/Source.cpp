#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

#include "../lab1/employee.h"

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: Reporter <binary_file> <report_file> <hourly_rate>\n";
        return 1;
    }

    char* binaryFile = argv[1];
    char* reportFile = argv[2];
    double hourlyRate = atof(argv[3]);

    if (hourlyRate <= 0)
    {
        std::cerr << "Error: hourly rate must be positive\n";
        return 1;
    }

    std::ifstream binFile(binaryFile, std::ios::binary);
    if (!binFile)
    {
        std::cerr << "Error: cannot open file " << binaryFile << "\n";
        return 1;
    }

    std::ofstream repFile(reportFile);
    if (!repFile)
    {
        std::cerr << "Error: cannot create report file " << reportFile << "\n";
        binFile.close();
        return 1;
    }

    std::cout << "Reporter: Creating report...\n";

    repFile << "File report \"" << binaryFile << "\"\n";
    repFile << std::string(70, '-') << "\n";
    repFile << std::left << std::setw(10) << "Number"
        << std::setw(15) << "Name"
        << std::setw(15) << "Hours"
        << std::setw(15) << "Salary\n";
    repFile << std::string(70, '-') << "\n";

    employee emp;
    int count = 0;

    while (binFile.read((char*)&emp, sizeof(employee)))
    {
        double salary = emp.hours * hourlyRate;

        repFile << std::left << std::setw(10) << emp.num
            << std::setw(15) << emp.name
            << std::setw(15) << std::fixed << std::setprecision(2) << emp.hours
            << std::setw(15) << std::fixed << std::setprecision(2) << salary << "\n";

        count++;
    }

    repFile << std::string(70, '-') << "\n";
    repFile << "Total employees: " << count << "\n";

    binFile.close();
    repFile.close();

    std::cout << "Reporter: Report created successfully!\n";
    std::cout << "Reporter: Total employees processed: " << count << "\n";

    return 0;
}