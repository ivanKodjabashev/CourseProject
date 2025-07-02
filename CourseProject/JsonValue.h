#pragma once
#include <iostream>
#include "JsonVisitor.h"


/**
 * @class JsonValue
 * @brief Абстрактна базова класа за всички JSON стойности
 */
class JsonValue
{
public:
    virtual void print() const = 0;
    virtual JsonValue* clone() const = 0;
    virtual void accept(JsonVisitor& visitor) const = 0;
    virtual ~JsonValue() = default;

	virtual JsonObject* asObject() { return nullptr; }
	virtual JsonArray* asArray() { return nullptr; }    
};