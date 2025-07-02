#pragma once
#include "JsonValue.h"
#include "JsonVisitor.h"

/**
 * @class JsonNull
 * @brief Представлява JSON null
 */
class JsonNull : public JsonValue
{
    // Празна стойност
public:
    JsonNull() = default;
    void print() const override;
    void accept(JsonVisitor& visitor) const override;
    JsonValue* clone() const override;
    ~JsonNull() override;
};