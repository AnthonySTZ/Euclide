#pragma once

namespace euclide {

enum class AttributeType {
    ATTR_TYPE_FLOAT,
    ATTR_TYPE_UNDEFINED,
};

template <typename T>
struct AttributeTypeOf {
    static constexpr AttributeType value = AttributeType::ATTR_TYPE_UNDEFINED;
};

template <>
struct AttributeTypeOf<float> {
    static constexpr AttributeType value = AttributeType::ATTR_TYPE_FLOAT;
};

} // namespace euclide