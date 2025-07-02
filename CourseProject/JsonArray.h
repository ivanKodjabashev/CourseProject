#include "JsonValue.h"


/**
 * @class JsonArray
 * @brief Представлява JSON масив
 */
class JsonArray : public JsonValue
{
    // Списък от JSON стойности
    JsonValue **ptr;
    unsigned capacity;
    unsigned current;

    void resize();
    void free();
    void copyFrom(const JsonArray &other);

public:
    JsonArray();
    JsonArray(const JsonArray &other);
    JsonArray &operator=(const JsonArray &other);
    ~JsonArray();
    
    void accept(JsonVisitor& visitor) const override;
    unsigned getSize() const;
    void addElement(const JsonValue &el);
    void remove(int index);
    JsonValue &operator[](int index);
    const JsonValue &operator[](int index) const;
    void print() const override;
	void setAt(int index, const JsonValue &value);
    JsonValue *clone() const override;

	JsonArray* asArray() override { return this; }
};
