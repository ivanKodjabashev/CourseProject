#pragma once
#include "JsonValue.h"
#include "JsonVisitor.h"

/**
 * @class JsonBool
 * @brief Представлява JSON булева стойност (true/false)
 */
class JsonBool : public JsonValue
{
    // Булева стойност
    bool value;

public:
    JsonBool(bool value);
    void print() const override;
    void accept(JsonVisitor& visitor) const override;
    JsonValue* clone() const override;
    void change();
    bool getBool() const;
};
