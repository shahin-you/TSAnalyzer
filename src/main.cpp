#include <iostream>

void printUsage(char *argv[]) 
{
    std::cerr << "Usage: " << argv[0] << " <input m2t file>" << std::endl;
}

int main(int argc, char *argv[]) 
{
    //read inout file from command line
    if (argc != 2) 
    {
        printUsage(argv);
        return 0;
    }
    return 0;
}   