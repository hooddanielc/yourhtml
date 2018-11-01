#include <iostream>

int main()
{
    setlocale(LC_ALL, "");
    std::cout << "What's your name? ";
    std::string name;
    std::getline(std::cin, name);
    std::cout << "Hello there, " << name << "." << std::endl;
    return 0;
}