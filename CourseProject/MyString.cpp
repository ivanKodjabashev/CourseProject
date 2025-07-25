#include "MyString.h"
#include <cstring>
#include <algorithm>
#pragma warning(disable : 4996)

static unsigned roundToPowerOfTwo(unsigned v) // static! -> will be discused in the next lecture
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

static unsigned dataToAllocByStringLen(unsigned stringLength)
{
    return std::max(roundToPowerOfTwo(stringLength + 1), 16u);
}

MyString::MyString() : MyString("") {}

MyString::MyString(const char* data)
{
    _size = std::strlen(data);
    _allocatedDataSize = dataToAllocByStringLen(_size);
    _data = new char[_allocatedDataSize];
    std::strcpy(_data, data);
}

MyString::MyString(size_t stringLength)
{
    _allocatedDataSize = dataToAllocByStringLen(stringLength);
    _data = new char[_allocatedDataSize];
    _size = 0;
    _data[0] = '\0';
}

MyString::MyString(const MyString& other)
{
    copyFrom(other);
}

MyString& MyString::operator=(const MyString& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

MyString::~MyString()
{
    free();
}

size_t MyString::getCapacity() const
{
    return _allocatedDataSize - 1;
}

size_t MyString::getSize() const
{
    return _size;
}

const char* MyString::c_str() const
{
    return _data;
}

MyString& MyString::operator+=(const MyString& other)
{
    if (getSize() + other.getSize() + 1 > _allocatedDataSize)
        resize(dataToAllocByStringLen(getSize() + other.getSize()));

    // we need to use strncat instead of strcat, because strcat will not work for str += str
    // (the terminating zero of str will be destroyed by the first char)
    std::strncat(_data, other._data, other.getSize());

    _size = getSize() + other.getSize();
    return *this;
}

char& MyString::operator[](size_t index)
{
    return _data[index]; // no security check!!
}

const char& MyString::operator[](size_t index) const
{
    return _data[index]; // no security check!!
}

std::ostream& operator<<(std::ostream& os, const MyString& obj)
{
    return os << obj._data;
}

std::istream& operator>>(std::istream& is, MyString& ref)
{
    char buff[1024];
    is >> buff;
    size_t buffStringSize = std::strlen(buff);

    if (buffStringSize > ref.getCapacity())
        ref.resize(dataToAllocByStringLen(buffStringSize));

    strcpy(ref._data, buff);
    ref._size = buffStringSize;
    return is;
}

void MyString::resize(unsigned newAllocatedDataSize)
{
    char* newData = new char[newAllocatedDataSize + 1];
    std::strcpy(newData, _data);
    delete[] _data;
    _data = newData;
    _allocatedDataSize = newAllocatedDataSize;
}

void MyString::free()
{
    delete[] _data;
}

void MyString::copyFrom(const MyString& other)
{
    _allocatedDataSize = other._allocatedDataSize;
    _data = new char[_allocatedDataSize];
    std::strcpy(_data, other._data);
    _size = other._size;
}

MyString operator+(const MyString& lhs, const MyString& rhs)
{
    MyString result(lhs.getSize() + rhs.getSize());
    result += lhs; // no resize is needed
    result += rhs;
    return result;
}

bool operator==(const MyString& lhs, const MyString& rhs)
{
    return std::strcmp(lhs.c_str(), rhs.c_str()) == 0;
}

bool operator!=(const MyString& lhs, const MyString& rhs)
{
    return std::strcmp(lhs.c_str(), rhs.c_str()) != 0;
}

bool operator<(const MyString& lhs, const MyString& rhs)
{
    return std::strcmp(lhs.c_str(), rhs.c_str()) < 0;
}

bool operator<=(const MyString& lhs, const MyString& rhs)
{
    return std::strcmp(lhs.c_str(), rhs.c_str()) <= 0;
}

bool operator>(const MyString& lhs, const MyString& rhs)
{
    return std::strcmp(lhs.c_str(), rhs.c_str()) > 0;
}

bool operator>=(const MyString& lhs, const MyString& rhs)
{
    return std::strcmp(lhs.c_str(), rhs.c_str()) >= 0;
}

MyString** MyString::split(char separetor)
{
    size_t tokenCount = 1;
    for (size_t i = 0; i < _size; ++i) {
        if (_data[i] == separetor) {
            ++tokenCount;
        }
    }
    
    MyString** result = new MyString*[tokenCount + 1];
    size_t start = 0;
    size_t idx = 0;
    
    for (size_t i = 0; i <= _size; ++i) {
        if (i == _size || _data[i] == separetor) {
            size_t len = i - start;
            char* buffer = new char[len + 1];
            for (size_t j = 0; j < len; ++j) {
                buffer[j] = _data[start + j];
            }
            buffer[len] = '\0';
            
            result[idx++] = new MyString(buffer);
            delete[] buffer;
            start = i + 1;
        }
    }
    result[idx] = nullptr;
    return result;
};