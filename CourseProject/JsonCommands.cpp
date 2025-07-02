#include "JsonCommands.h"
#include "Commands.h"
#include "JsonReader.h"
#include "JsonPrinter.h"
#include "JsonHelper.h"
#include <fstream>
#include <stdexcept>

void JsonCommandProccesor::executeCommand(MyString& text)
{
    MyString** commands = text.split(' ');
    int count = JsonHelper::countSplittedSize(commands);
    Commands helper;

    if (count == 0) {
        JsonHelper::free(commands, count);
        return;
    }

    MyString& action = *commands[0];

    if (action == "open") {
        if (count < 2) {
            throw std::runtime_error("Missing filename");
        }

        MyString newFile = *commands[1];

        std::ifstream in(newFile.c_str());
        if (!in.is_open())
        {
            throw std::runtime_error("Cannot open file");
        }
		JsonReader reader;
        JsonValue* newRoot = reader.parse(in);
        if (!newRoot)
        {
            throw std::runtime_error("cannot parse Json");
        }

        delete root;
        root = newRoot;
        filename = newFile;
    }
    else if (action == "move") 
    {
        if (count<3)
        {
            throw std::runtime_error("Missings paths for move");
        }
        helper.moveElement(root, *commands[1], *commands[2]);
    }
    else if (action == "validate") {
        helper.validate(root);
    }
    else if (action == "print") {
        helper.print(root);
    }
    else if (action == "search") {
        if (count < 2)
            throw std::runtime_error("Missing search element");  
        helper.search(root, *commands[1]);  
    }
    else if (action == "set") {
        if (count < 3)
            throw std::runtime_error("Missing parameters for set");  
        helper.set(root, *commands[1], *commands[2]);
    }
    else if (action == "create") {
        if (count < 3)
            throw std::runtime_error("Missing parameters for create"); 
        helper.create(root, *commands[1], *commands[2]);
    }
    else if (action == "delete") {
        if (count < 2)
            throw std::runtime_error("Missing path for delete");  
        helper.deleteElement(root, *commands[1]);
    }
    else if (action == "save") {
       
        std::ofstream out(this->filename.c_str());
        if (!out.is_open())
            throw std::runtime_error("Cannot open file to save");
        JsonPrinter printer(out);
        root->accept(printer);
    }
    else if (action == "saveAs") {
        if (count < 2)
            throw std::runtime_error("Missing filename for saveAs");  
        std::ofstream out(commands[1]->c_str());
        if (!out.is_open())
            throw std::runtime_error("Cannot open file to saveAs");
        JsonPrinter printer(out);
        root->accept(printer);
        filename = *commands[1];
    }else if(action=="exit")
    {
        this->isClosed = true;
    }
    else {
        throw std::runtime_error("Invalid command"); 
    }

    JsonHelper::free(commands, count); 
}

JsonCommandProccesor::JsonCommandProccesor()
{
    this->root = nullptr;
	this->filename = "";
    this->isClosed = false;

}

JsonCommandProccesor::~JsonCommandProccesor()
{
    delete root;
}

bool JsonCommandProccesor::exitRequested() const
{
    return this->isClosed;
}