#include <iostream>
#include "database_singleton.h"
#include "abstract_handler.h"
#include "commands/command.h"
#include <fstream>

class key_comparer
{
public:
    int operator()(int first, int second)
    {
        return first - second;
    }

    int operator()(std::string first, std::string second)
    {
        if (first > second)
            return 1;
        else if (first < second)
            return -1;
        else
            return 0;
    }
};

int main(int argc, char *argv[])
{

    // database_singleton<int, std::string, key_comparer> *db = new database_singleton<int, std::string, key_comparer>();

    abstract_handler<student_definition> *db = new database_singleton<int, std::string, key_comparer>();

    // std::string command;

    // std::ifstream file("../tests/file_to_test.txt");

    // while (std::getline(file, command))
    // {
    //     db->handle_request(command);
    // }

    delete db;

    // getchar();

    return 0;
}