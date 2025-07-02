#include "JsonReader.h"
#include "JsonString.h"
#include "JsonNumber.h"
#include "JsonBool.h"
#include "JsonArray.h"
#include "JsonObject.h"
#include "JsonNull.h"
#include "JsonHelper.h"

JsonValue* JsonReader::parse(std::istream& is)
{
    avoidSpace(is);

    char letter = this->peek(is);

    if (letter == '{')
    {
        return this->parseObject(is);
    }
    else if (letter == '[')
    {
        return this->parseArray(is);
    }
    else if ((letter >= '0' && letter <= '9') || letter == '-')
    {
        return this->parseNumber(is);
    }
    else if (letter == '"')
    {
        return this->parseString(is);
    }
    else if (letter == 't' || letter == 'f')
    {
        return this->parseBool(is);
    }
    else if (letter == 'n')
    {
        return this->parseNull(is);
    }

    throw std::runtime_error("that is not a valid JSON");
}

void JsonReader::avoidSpace(std::istream& is)
{
	char ch = is.peek();
    while (ch == ' ' || ch == '\n' || ch == '\r'|| ch == '\t')
    {
        is.get();
		ch = is.peek();
    }
}

char JsonReader::peek(std::istream& is)
{
    avoidSpace(is);

    return is.peek();
}

JsonValue* JsonReader::parseNull(std::istream& is)
{
    MyString text = "";
    for (size_t i = 0; i < 4; i++)
    {
		char buf[2] = { (char)is.get(), '\0' };
        text += buf;
    }

    if (text != "null")
    {
        throw std::runtime_error("Invalid Null");
    }

    return new JsonNull();
}

JsonValue* JsonReader::parseString(std::istream& is)
{
    MyString text = "";
    char temp;
    is.get();
    while ((temp = is.get()) != '"')
    {
        if (is.eof())
        {
            throw std::runtime_error("Invalid string");
        }

        char buf[2] = { temp, '\0' };
		text += buf;
    }

    return new JsonString(text);
}

JsonValue* JsonReader::parseBool(std::istream& is)
{
    char ch = is.peek();

    if (ch == 't')
    {
        MyString text = "";

        for (size_t i = 0; i < 4; i++)
        {
            char buf[2] = { (char)is.get(), '\0' };
            text += buf;
        }
        if (text == "true")
        {
            return new JsonBool(true);
        }
    }
    else if (ch == 'f')
    {
        MyString text = "";

        for (size_t i = 0; i < 5; i++)
        {
            char buf[2] = { (char)is.get(), '\0' };
            text += buf;
        }

        if (text == "false")
        {
            return new JsonBool(false);
        }
    }

    throw std::runtime_error("Invalid bool");
}

JsonValue* JsonReader::parseNumber(std::istream& is)
{
    MyString text = "";

    if (is.peek() == '-')
    {
        char buf[2] = { (char)is.get(), '\0' };
        text += buf;
    }

    while (is.peek() >= '0' && is.peek() <= '9')
    {
		char buf[2] = { (char)is.get(), '\0' };
        text += buf;
    }
    return new JsonNumber(JsonHelper::toInt(text));
}


JsonValue* JsonReader::parseArray(std::istream& is)
{
    avoidSpace(is);
    if (is.peek() != '[')
    {
        throw std::runtime_error("Invalid array");
    }

    is.get();

    JsonArray* arr = new JsonArray();

    avoidSpace(is);
    if (is.peek() == ']')
    {
        is.get();
        return arr;
    }
	arr->addElement(*this->parse(is));
	avoidSpace(is);
    while (is.peek() == ',')
    {
        char temp = is.get();
		avoidSpace(is);
        arr->addElement(*this->parse(is));
        avoidSpace(is);
    }
    if (is.get() != ']') 
    {
		throw std::runtime_error("Invalid array");
    };

    return arr;
}

JsonValue* JsonReader::parseObject(std::istream& is)
{
    avoidSpace(is);
    if (is.peek() != '{')
    {
        throw std::runtime_error("Invalid object");
    }

    is.get();
    JsonObject* obj = new JsonObject();

    if (is.peek() == '}')
    {
        is.get();
        return obj;
    }

    while (is.peek() != '}')
    {
        avoidSpace(is);
        JsonString* key = (JsonString*)this->parseString(is);
        avoidSpace(is);
        if (is.peek() != ':')
        {
			delete key;
            delete obj;
            throw std::runtime_error("Invalid object");
        }

		is.get();
		avoidSpace(is);
        obj->addElement(key->getText(), *this->parse(is));
		delete key;
		avoidSpace(is);
		char c = is.peek();
        if (is.peek() == ',')
        {
			is.get();
			avoidSpace(is);
            continue;
        }
        else if (is.peek() == '}')
        {
            is.get();
            break;
        }else 
        {
			delete obj;
            throw std::runtime_error("Invalid object");
		}
    }
    return obj;
}
