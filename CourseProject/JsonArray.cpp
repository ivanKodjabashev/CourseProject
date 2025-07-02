#include "JsonArray.h"
#include <iostream>
#include <stdexcept>


void JsonArray::resize() {
    unsigned newCap = (capacity == 0 ? 2 : capacity * 2);
    JsonValue** newPtr = new JsonValue * [newCap];
    for (unsigned i = 0; i < current; i++) {
        newPtr[i] = ptr[i];
    }
    delete[] ptr;
    ptr = newPtr;
    capacity = newCap;
}


void JsonArray::free() {
    for (unsigned i = 0; i < current; i++) {
        delete ptr[i];
    }
    delete[] ptr;
    ptr = nullptr;
    capacity = 0;
    current = 0;
}


void JsonArray::copyFrom(const JsonArray& other) {
    capacity = other.capacity;
    current = other.current;
    ptr = new JsonValue * [capacity];
    for (unsigned i = 0; i < current; i++) {
        ptr[i] = other.ptr[i]->clone();
    }
}

JsonArray::JsonArray()
{
	this->capacity = 0;
	this->current = 0;
	this->ptr = nullptr;
    resize();
}

JsonArray::JsonArray(const JsonArray& other)
{
    copyFrom(other);
}

JsonArray& JsonArray::operator=(const JsonArray& other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

JsonArray::~JsonArray() {
    free();
}

void JsonArray::accept(JsonVisitor& visitor) const {
    visitor.visit(*this);
}

unsigned JsonArray::getSize() const {
    return current;
}

void JsonArray::addElement(const JsonValue& el) {
    if (current == capacity) {
        resize();
    }
    ptr[current++] = el.clone();
}

void JsonArray::remove(int index) {
    if (index < 0 || index >= current) {
        throw std::runtime_error("index out of range");
    }
    delete ptr[index];   
}

JsonValue& JsonArray::operator[](int index) {
    if (index < 0 || index >= current) {
        throw std::runtime_error("index out of range");
    }
    return *ptr[index];
}

const JsonValue& JsonArray::operator[](int index) const {
    if (index < 0 || index >= current) {
        throw std::runtime_error("index out of range");
    }
    return *ptr[index];
}

void JsonArray::print() const {
    std::cout << '[';
    for (unsigned i = 0; i < current; ++i) {
        ptr[i]->print();
        if (i + 1 < current) {
            std::cout << ',';
        }
    }
    std::cout << ']';
}

void JsonArray::setAt(int index, const JsonValue& value) {
    if (index < 0 || index >= current) {
        throw std::runtime_error("index out of range");
    }
    delete ptr[index];
    ptr[index] = value.clone();
}

JsonValue* JsonArray::clone() const {
    return new JsonArray(*this);
}
