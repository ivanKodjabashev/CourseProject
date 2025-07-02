#pragma once

/**
 * @interface JsonVisitor
 * @brief Interface for visiting JSON value objects (Visitor Pattern).
 */

class JsonNumber;
class JsonString;
class JsonBool;
class JsonNull;
class JsonArray;
class JsonObject;

class JsonVisitor {
public:
    virtual ~JsonVisitor() = default;

    virtual void visit(const JsonNull& null) = 0;
    virtual void visit(const JsonBool& boolean) = 0;
    virtual void visit(const JsonNumber& num) = 0;
    virtual void visit(const JsonString& str) = 0;
    virtual void visit(const JsonArray& arr) = 0;
    virtual void visit(const JsonObject& obj) = 0;
};