#include "JsonString.h"

JsonString::JsonString(const MyString& text)
{
    this->text = text;
}

JsonValue* JsonString::clone() const
{
    return new JsonString(this->text);
}

void JsonString::print() const
{
    std::cout << this->text.c_str() << std::endl;
}

void JsonString::accept(JsonVisitor& visitor) const
{
    visitor.visit(*this);
}

MyString JsonString::getText() const
{
    return this->text;
}