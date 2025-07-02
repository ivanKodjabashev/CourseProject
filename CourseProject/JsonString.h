#pragma once
#include "JsonValue.h"
#include "MyString.h"

/**
 * @class JsonString
 * @brief Представлява JSON низ
 */

class JsonString : public JsonValue
{
    // Стойност от тип низ
    MyString text;

public:
    MyString getText() const;
    JsonString(const MyString& text);
    void print() const override;
    void accept(JsonVisitor& visitor) const override;
    JsonValue* clone() const override;
};