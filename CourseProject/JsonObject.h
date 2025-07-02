#pragma once
#include "JsonValue.h"
#include "MyString.h"
#include "JsonVisitor.h"


/**
 * @class JsonObject
 * @brief Представлява JSON обект с ключове и стойности
 */
class JsonObject : public JsonValue
{
    // JSON обект съхраняващ двойки ключ-стойност
    MyString* key;
    JsonValue** value;

    unsigned capacity;
    unsigned current;

    void resize();
    void free();
    void copyFrom(const JsonObject& other);

public:
    JsonObject();
    JsonObject(const JsonObject& other);
    ~JsonObject();
    JsonObject& operator=(const JsonObject& other);

    MyString* getKeys() const;
    void accept(JsonVisitor& visitor) const override;
    void addElement(const MyString& key, const JsonValue& value);
    bool contains(const MyString& key) const;
    void remove(const MyString& key);
    JsonValue* get(const MyString& key) const;
    void print() const override;
    JsonValue* clone() const override;
    unsigned getSize() const;

	JsonObject* asObject() override { return this; }
};