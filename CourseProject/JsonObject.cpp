#include "JsonObject.h"

JsonObject::JsonObject()
{
    this->capacity = 0;
    this->current = 0;
    this->key = nullptr;
    this->value = nullptr;
}

void JsonObject::free()
{
    this->capacity = 0;
    this->current = 0;
    delete[] this->key;
    for (size_t i = 0; i < this->current; i++)
    {
        delete this->value[i];
    }

    delete[] this->value;
}

void JsonObject::copyFrom(const JsonObject& other)
{
    this->capacity = other.capacity;
    this->current = other.current;
    this->key = new MyString[other.capacity];
    this->value = new JsonValue * [other.capacity];
    for (size_t i = 0; i < this->current; i++)
    {
        this->key[i] = other.key[i];
        this->value[i] = other.value[i]->clone();
    }
}

void JsonObject::resize()
{
    if (capacity == 0)
    {
        this->capacity = 2;
    }
    else
    {
        this->capacity *= 2;
    }

    MyString* newKey = new MyString[this->capacity];
    JsonValue** newValue = new JsonValue * [this->capacity];

    for (size_t i = 0; i < this->current; i++)
    {
        newKey[i] = key[i];
        newValue[i] = value[i];
    }

    delete[] this->key;
    delete[] this->value;

    this->key = newKey;
    this->value = newValue;
}

JsonObject::JsonObject(const JsonObject& other)
{
    this->copyFrom(other);
}

JsonObject::~JsonObject()
{
    free();
}

JsonObject& JsonObject::operator=(const JsonObject& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }

    return *this;
}

void JsonObject::addElement(const MyString& key, const JsonValue& value)
{
    if (this->contains(key))
    {
        for (size_t i = 0; i < this->current; i++)
        {
            if (this->key[i] == key)
            {
                delete this->value[i];
                this->value[i] = value.clone();
                return;
            }
        }
    }

    if (this->current == this->capacity)
    {
        resize();
    }

    this->key[this->current] = key;
    this->value[this->current++] = value.clone();
}

bool JsonObject::contains(const MyString& key) const
{
    return get(key) != nullptr;
}

JsonValue* JsonObject::get(const MyString& key) const
{
    for (size_t i = 0; i < this->current; i++)
    {
        if (key == this->key[i])
        {
            return this->value[i];
        }
    }

    return nullptr;
}

unsigned JsonObject::getSize() const
{
    return this->current;
}

JsonValue* JsonObject::clone() const
{
    return new JsonObject(*this);
}

void JsonObject::remove(const MyString& text)
{
    for (size_t i = 0; i < this->current; i++)
    {
        if (text == this->key[i])
        {
            for (size_t j = i; j < this->current - 1; j++)
            {
                std::swap(this->key[j], this->key[j + 1]);
                std::swap(this->value[j], this->value[j + 1]);
            }
            this->current--;
            break;
        }
    }
}

void JsonObject::print() const
{
    for (size_t i = 0; i < this->current; i++)
    {
        std::cout << this->key[i] << ": " << this->value[i] << std::endl;
    }
}

void JsonObject::accept(JsonVisitor& visitor) const
{
    visitor.visit(*this);
}

MyString* JsonObject::getKeys() const
{
    return this->key;
}

