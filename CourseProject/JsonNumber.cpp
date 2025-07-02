#include "JsonNumber.h"

JsonNumber::JsonNumber(int n)
{
    this->number = n;
}

void JsonNumber::print() const
{
    std::cout << this->number << std::endl;
}

JsonValue* JsonNumber::clone() const
{
    return new JsonNumber(this->number);
}

int JsonNumber::getNumber() const
{
    return this->number;
}

void JsonNumber::accept(JsonVisitor& visitor) const
{
    visitor.visit(*this);
}
