#include "attribute_set.h"

#include <utility>

namespace euclide {

AttributeSet::AttributeSet(const AttributeSet& t_other) {
    copy(t_other);
}

AttributeSet& AttributeSet::operator=(const AttributeSet& t_other) {
    if (this == &t_other)
        return *this;

    m_attributes.clear();
    m_map.clear();

    copy(t_other);

    return *this;
}

const Attribute* AttributeSet::find(const std::string& t_name) const {
    auto it = m_map.find(t_name);
    return it == m_map.end() ? nullptr : m_attributes[it->second].get();
}

Attribute* AttributeSet::find(const std::string& t_name) {
    return const_cast<Attribute*>(std::as_const(*this).find(t_name));
}

const Attribute* AttributeSet::get(size_t index) const {
    if (index >= m_attributes.size())
        return nullptr;

    return m_attributes[index].get();
}

Attribute* AttributeSet::get(size_t index) {
    return const_cast<Attribute*>(std::as_const(*this).get(index));
}

void AttributeSet::resize(const size_t t_size) {
    for (auto& attr : m_attributes) {
        attr->resize(t_size);
    }
    m_size = t_size;
}

void AttributeSet::copy(const AttributeSet& t_other) {
    m_attributes.reserve(t_other.m_attributes.size());
    for (auto& attr : t_other.m_attributes)
        m_attributes.push_back(attr->clone());

    m_map = t_other.m_map;
    m_size = t_other.m_size;
}

} // namespace euclide