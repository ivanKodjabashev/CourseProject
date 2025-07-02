#pragma once
#include <iostream>
#include "JsonValue.h"
#include "MyString.h"


/**
 * @class JsonCommandProccesor
 * @brief Интерпретира и изпълнява низови команди върху JSON дървото
 */
class JsonCommandProccesor
{
    JsonValue* root;
	MyString filename;
    bool isClosed;
public:
    JsonCommandProccesor();
    ~JsonCommandProccesor();

    bool exitRequested() const;

    void executeCommand(MyString& command);
};