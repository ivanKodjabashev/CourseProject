#pragma once
#include <iostream>
#include "JsonValue.h"

/**
 * @class JsonReader
 * @brief Отговаря за парсирането на JSON текст до дървовидна структура от JsonValue обекти
 */
class JsonReader
{
    char peek(std::istream& is);
    void avoidSpace(std::istream& in);
    JsonValue* parseValue(std::istream& in);
    JsonValue* parseNull(std::istream& in);
    JsonValue* parseBool(std::istream& in);
    JsonValue* parseNumber(std::istream& in);
    JsonValue* parseString(std::istream& in);
    JsonValue* parseArray(std::istream& in);
    JsonValue* parseObject(std::istream& in);

public:
    JsonValue* parse(std::istream& is);
};