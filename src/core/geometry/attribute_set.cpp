#include "attribute_set.h"

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

Attribute* AttributeSet::find(const std::string& t_name) {
    auto it = m_map.find(t_name);
    return it == m_map.end() ? nullptr : m_attributes[it->second].get();
}

Attribute* AttributeSet::get(const size_t t_index) {
    if (t_index >= m_attributes.size())
        return nullptr;

    return m_attributes[t_index].get();
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