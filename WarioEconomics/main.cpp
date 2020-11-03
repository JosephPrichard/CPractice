#include <iostream>
#include <fstream>
#include "LList.h"

int main() {

    std::string input_str;
    std::cin >> input_str;
    std::ifstream input(input_str, std::ios::binary);
    if(!input) {
        std::cout << "Input file does not exist." << std::endl;
        return 0;
    }
    std::ofstream output("derive.txt" , std::ios::trunc | std::ios::binary);
    if(!output) {
        std::cout << "Output file does not exist." << std::endl;
        return 0;
    }

    auto* list = new LList;

    std::string equation;
    while(getline(input,equation)) {
        list->deleteNodes();
        *list >> equation;
        list->sort();
        list->differentiate();
        list->purge();
        std::cout << *list << std::endl;
        output << *list << std::endl;
    }

    input.close();
    output.close();
    delete list;

    return 0;
}