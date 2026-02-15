#include <iostream>
#include <fstream>
#include <cstring>

#include "../lab1/employee.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: Creator <filename> <record_count>\n";
        return 1;
    }

    char* filename = argv[1];
    int recordCount = atoi(argv[2]);

    if (recordCount <= 0)
    {
        std::cerr << "Error: record count must be positive\n";
        return 1;
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Error: cannot create file " << filename << "\n";
        return 1;
    }

    std::cout << "Creator: Creating file " << filename << " with " << recordCount << " records" << "\n";
    std::cout << "Enter employee data:\n";

    employee emp;

    for (int i = 0; i < recordCount; i++)
    {
        std::cout << "\nEmployee #" << (i + 1) << ":\n";

        std::cout << "\tID number: ";
        std::cin >> emp.num;

        std::cout << "\tName (max 9 chars): ";
        std::cin >> emp.name;

        std::cout << "\tHours worked: ";
        std::cin >> emp.hours;

        file.write((char*)&emp, sizeof(employee));
    }

    file.close();
    std::cout << "\nCreator: File created successfully!\n";

    return 0;
}