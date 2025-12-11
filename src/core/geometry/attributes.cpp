#include "attributes.h"

namespace euclide {

Attribute::~Attribute() {
    switch (type) {
    case AttributeType::ATTR_TYPE_FLOAT:
        for (size_t i = 0; i < attrSize; ++i)
            if (data[i])
                free(data[i]);
    case AttributeType::ATTR_TYPE_STRING:
        std::string* arr = static_cast<std::string*>(data[0]);
        if (arr)
            delete[] arr;
    }
}

} // namespace euclide