#include "JsonNull.h"

void JsonNull::print() const
{
    std::cout << "null" << std::endl;
}

JsonValue* JsonNull::clone() const
{
    return new JsonNull();
}

JsonNull::~JsonNull() {};

void JsonNull::accept(JsonVisitor& visitor) const
{
    visitor.visit(*this);
}