#include <iostream>
#include <string>
#include "JsonCommands.h"
#include "MyString.h"

int main() {
    JsonCommandProccesor processor;
    std::cout << " Нашиши open <filepath>, за да заредиш Json, и после въвеждай команди.С exit се излиза.\n";

    const int MAX_LINE = 1024;
    char buffer[MAX_LINE];

    while (true) {
        std::cout << "> ";
        if (!std::cin.getline(buffer, MAX_LINE)) {
            break;  
        }
        if (buffer[0] == '\0') continue;

        MyString command(buffer);
        try {
            processor.executeCommand(command);
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        if (processor.exitRequested()) {
            std::cout << "Exiting\n";
            break;
        }
    }
    return 0;
}