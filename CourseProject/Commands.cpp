#include "JsonArray.h"
#include "Commands.h"
#include "JsonReader.h"
#include "JsonString.h"
#include "JsonPrinter.h"
#include "JsonObject.h"
#include <stdexcept>  
#include "JsonHelper.h"

void Commands::search(JsonValue* root, const MyString& key) {
    if (!isValid(root))
        throw std::runtime_error("invalid JSON");

    JsonArray results;
    collectMatches(root, key, results);

    if (results.getSize() == 0)
        throw std::runtime_error("No matches");

    JsonPrinter printer(std::cout);
    results.accept(printer);
}

JsonValue* Commands::getPath(JsonValue* current, MyString * *pathSplitted, int countSplitted) {
    for (int i = 0; i < countSplitted - 1; i++) {
        JsonObject* obj = current->asObject();
        if (obj != nullptr) {
            if (!obj->contains(*pathSplitted[i])) {
                JsonHelper::free(pathSplitted, countSplitted);
                throw std::runtime_error("invalid path");
            }
            current = obj->get(*pathSplitted[i]);
        }
        else {
            JsonArray* arr = current->asArray();
            if (arr != nullptr) {
                int index = JsonHelper::toInt(*pathSplitted[i]);
                if (index < 0 || index >= arr->getSize()) {
                    JsonHelper::free(pathSplitted, countSplitted);
                    throw std::runtime_error("invalid path");
                }
                current = &(*arr)[index];
            }
            else {
                JsonHelper::free(pathSplitted, countSplitted);
                throw std::runtime_error("invalid path");
            }
        }
    }
    return current;
}

void Commands::collectMatches(JsonValue* root, const MyString& key, JsonArray& out) const
{
    JsonObject* obj = root->asObject();
    if (obj!=nullptr) {
        size_t n = obj->getSize();
        MyString* keys = obj->getKeys();
        for (size_t i = 0; i < n; i++) {
            if (keys[i] == key) {
                out.addElement(*obj->get(keys[i]));
            }
            collectMatches(obj->get(keys[i]), key, out);
        }
    }
    else {
        JsonArray* arr = root->asArray();
        if (arr!=nullptr)
        {
            for (unsigned i = 0; i < arr->getSize(); i++) {
                collectMatches(&(*arr)[i], key, out);
            }
        }
    }
}



bool Commands::isValid(JsonValue* root)
{
    return root != nullptr;
}

void Commands::validate(JsonValue* root)
{
    std::cout << (this->isValid(root) ? "It is valid" : "It is not valid") << std::endl;
}

void Commands::print(JsonValue* root)
{
    if (isValid(root))
    {
        JsonPrinter printer(std::cout);
        root->accept(printer);
    }
    else
    {
        std::cout << "It cannot be printed" << std::endl;
    }
}

void Commands::set(JsonValue* root, MyString& path, const MyString& text)
{
    if (!isValid(root))
    {
        throw std::runtime_error("Invalid json");  
    }

    MyString** pathSplited = path.split('/');
    int countSplit = JsonHelper::countSplittedSize(pathSplited);
	JsonValue* current = getPath(root, pathSplited, countSplit);
    
    JsonArray* arr = current->asArray();
    if (arr != nullptr) {
        int index = JsonHelper::toInt(*pathSplited[countSplit - 1]);
        if (index < 0 || index >= arr->getSize()) {
            JsonHelper::free(pathSplited, countSplit);
            throw std::runtime_error("invalid path");
        }
        arr->setAt(index, JsonString(text));
    }
    else {
        JsonObject* obj = current->asObject();
        if (obj == nullptr) {
            JsonHelper::free(pathSplited, countSplit);
            throw std::runtime_error("invalid path");
        }
        obj->remove(*pathSplited[countSplit - 1]);
        obj->addElement(*pathSplited[countSplit - 1], JsonString(text));
    }
    JsonHelper::free(pathSplited, countSplit);  
}

void Commands::create(JsonValue* root, MyString& path, const MyString& text) {
    if (!isValid(root)) {
        throw std::runtime_error("Invalid json");
    }
    MyString** pathSplitted = path.split('/');
    int countSplited = JsonHelper::countSplittedSize(pathSplitted);
    JsonValue* current = getPath(root, pathSplitted, countSplited);

    JsonObject* objDest = current->asObject();
    if (objDest != nullptr) {
        if (objDest->contains(*pathSplitted[countSplited - 1])) {
            JsonHelper::free(pathSplitted, countSplited);
            throw std::runtime_error("Element already exists");
        }
        objDest->addElement(*pathSplitted[countSplited - 1], JsonString(text));
    }
    else {
        JsonArray* arrDest = current->asArray();
        if (arrDest != nullptr) {
            int index = JsonHelper::toInt(*pathSplitted[countSplited - 1]);
            if (index < 0 || index > arrDest->getSize()) {
                JsonHelper::free(pathSplitted, countSplited);
                throw std::runtime_error("invalid path");
            }
            if (index < arrDest->getSize()) {
                JsonHelper::free(pathSplitted, countSplited);
                throw std::runtime_error("Element already exists");
            }
            arrDest->addElement(JsonString(text));
        }
        else {
            JsonHelper::free(pathSplitted, countSplited);
            throw std::runtime_error("invalid path");
        }
    }
    JsonHelper::free(pathSplitted, countSplited);
}

void Commands::deleteElement(JsonValue* root, MyString& path)
{
    if (!isValid(root))
    {
        throw std::runtime_error("Invalid json");
    }

    MyString** pathSplited = path.split('/');
    int countSplited = JsonHelper::countSplittedSize(pathSplited);
	JsonValue* current = getPath(root, pathSplited, countSplited);

    JsonObject* obj = current->asObject();
    if (obj != nullptr) {
        if (!obj->contains(*pathSplited[countSplited - 1])) {
            JsonHelper::free(pathSplited, countSplited);
            throw std::runtime_error("invalid path");
        }
        obj->remove(*pathSplited[countSplited - 1]);
    }
    else {
        JsonArray* arr = current->asArray();
        if (arr != nullptr) {
            int index = JsonHelper::toInt(*pathSplited[countSplited - 1]);
            if (index < 0 || index >= arr->getSize()) {
                JsonHelper::free(pathSplited, countSplited);
                throw std::runtime_error("invalid path");
            }
            arr->remove(index);
        }
        else {
            JsonHelper::free(pathSplited, countSplited);
            throw std::runtime_error("invalid path");
        }
    }
    JsonHelper::free(pathSplited, countSplited);
}

void Commands::moveElement(JsonValue* root, MyString& from, MyString& to)
{
    if (!isValid(root))
    {
        throw std::runtime_error("Invalid json");
    }

    MyString** fromSplited = from.split('/');
    int countFromSplited = JsonHelper::countSplittedSize(fromSplited);
    JsonValue* fromPtr = root;
    JsonValue* toPtr = root;

	fromPtr = getPath(fromPtr, fromSplited, countFromSplited);

    JsonValue* fromElement = nullptr;

    JsonObject* fromObj = fromPtr->asObject();
    if (fromObj != nullptr)
    {
        if (!fromObj->contains(*fromSplited[countFromSplited - 1]))
        {
            JsonHelper::free(fromSplited, countFromSplited);
            throw std::runtime_error("invalid path");
        }
        fromElement = fromObj->get(*fromSplited[countFromSplited - 1])->clone();
        fromObj->remove(*fromSplited[countFromSplited - 1]);
    }
    else
    {
        JsonArray* fromArr = fromPtr->asArray();
        if (fromArr != nullptr)
        {
            int index = JsonHelper::toInt(*fromSplited[countFromSplited - 1]);
            if (index < 0 || index >= fromArr->getSize())
            {
                JsonHelper::free(fromSplited, countFromSplited);
                throw std::runtime_error("invalid path");
            }
            fromElement = (*fromArr)[index].clone();
            fromArr->remove(index);
        }
        else
        {
            JsonHelper::free(fromSplited, countFromSplited);
            throw std::runtime_error("invalid path");
        }
    }
    JsonHelper::free(fromSplited, countFromSplited);  

    
    MyString** toSplited = to.split('/');
    int countToSplited = JsonHelper::countSplittedSize(toSplited);
	toPtr = getPath(toPtr, toSplited, countToSplited);

    JsonObject* toObj = toPtr->asObject();
    if (toObj != nullptr)
    {
        if (toObj->contains(*toSplited[countToSplited - 1]))
        {
            delete fromElement;
            JsonHelper::free(toSplited, countToSplited);
            throw std::runtime_error("already exists");
        }
        toObj->addElement(*toSplited[countToSplited - 1], *fromElement);
    }
    else
    {
        JsonArray* arr = toPtr->asArray();
        if (arr == nullptr)
        {
            delete fromElement;
            JsonHelper::free(toSplited, countToSplited);
            throw std::runtime_error("invalid path");
        }
        int index = JsonHelper::toInt(*toSplited[countToSplited - 1]);
        if (index < 0 || index > arr->getSize())
        {
            delete fromElement;
            JsonHelper::free(toSplited, countToSplited);
            throw std::runtime_error("invalid path");
        }
        if (index == arr->getSize())
        {
            arr->addElement(*fromElement);
        }
        else
        {
            arr->setAt(index, *fromElement);
        }
    }
    delete fromElement; 
    JsonHelper::free(toSplited, countToSplited);
}

void Commands::save(JsonValue* root, const MyString& path)
{
    if (!isValid(root)) 
    {
        throw std::runtime_error("Invalid JSON");
    }

    std::ofstream of(path.c_str());
    if (!of.is_open())
    {
        throw std::runtime_error("Cannot save");
    }

    JsonPrinter printer(of);
    root->accept(printer);

}

void Commands::saveAs(JsonValue* root, const MyString& path) 
{
    save(root, path);
}