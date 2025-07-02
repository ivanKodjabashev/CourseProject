#include "JsonBool.h"

JsonBool::JsonBool(bool value)
{
    this->value = value;
}

void JsonBool::print() const
{
    std::cout << this->value << std::endl;
}

JsonValue* JsonBool::clone() const
{
    return new JsonBool(this->value);
}

void JsonBool::change()
{
    this->value = !this->value;
}

bool JsonBool::getBool() const
{
    return this->value;
}

void JsonBool::accept(JsonVisitor& visitor) const
{
    visitor.visit(*this);
}