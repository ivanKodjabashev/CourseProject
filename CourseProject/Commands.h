#pragma once
#include "MyString.h"
#include "JsonValue.h"
#include <fstream>

class Commands
{
    bool isValid(JsonValue* root);
	void collectMatches(JsonValue* cur, const MyString& key, JsonArray& out) const;
    JsonValue* getPath(JsonValue* cur, MyString** pathSplitted, int countSplited);
public:
    void validate(JsonValue* root);
    void print(JsonValue* root);
    void search(JsonValue* root, const MyString& wanted);
    void set(JsonValue* root, MyString& path, const MyString& text);
    void create(JsonValue* root, MyString& path, const MyString& text);
    void deleteElement(JsonValue* root, MyString& path);
    void moveElement(JsonValue* root, MyString& from, MyString& to);
    void save(JsonValue* root, const MyString& path);
    void saveAs(JsonValue* root, const MyString& path);
};