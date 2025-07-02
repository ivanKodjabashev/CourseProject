#pragma once
#include "MyString.h"
#include "JsonValue.h"
class JsonHelper
{
public:
    static void free(MyString** pathSplited, int count);
    static int countSplittedSize(MyString** pathSplited);
    static int toInt(const MyString& num);
};