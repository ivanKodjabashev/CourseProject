#include "JsonValue.h"
#include "JsonVisitor.h"

/**
 * @class JsonNumber
 * @brief Представлява JSON число
 */
class JsonNumber : public JsonValue
{
    // Стойност от тип число
    int number;

public:
    JsonNumber(int n);
    void print() const override;
    void accept(JsonVisitor& visitor) const override;
    JsonValue* clone() const override;
    int getNumber() const;
};
