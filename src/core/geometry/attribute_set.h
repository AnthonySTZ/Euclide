#pragma once

#include "attribute.h"
#include <iostream>

namespace euclide {

class AttributeSet {
  public:
    AttributeSet() = default;
    ~AttributeSet() = default;

    AttributeSet(const AttributeSet& t_other);
    AttributeSet& operator=(const AttributeSet& t_other);

    [[nodiscard]] const Attribute* find(const std::string& t_name) const;
    [[nodiscard]] Attribute* find(const std::string& t_name);

    template <typename T, size_t COMPONENTS>
    inline Attribute* findOrCreate(const std::string& t_name) {
        auto it = m_map.find(t_name);
        if (it != m_map.end())
            return m_attributes[it->second].get();

        size_t attrIndex = m_attributes.size();
        m_map.emplace(t_name, attrIndex);
        m_attributes.emplace_back(std::make_unique<TypedAttribute<T, COMPONENTS>>(t_name));
        auto* attr = m_attributes[attrIndex].get();
        attr->resize(m_size);
        return attr;
    }

    template <typename T>
    inline Attribute* findOrCreate(const std::string& t_name, const int t_attrSize) {
        switch (t_attrSize) {
        case 1:
            return findOrCreate<T, 1>(t_name);
        case 2:
            return findOrCreate<T, 2>(t_name);
        case 3:
            return findOrCreate<T, 3>(t_name);
        case 4:
            return findOrCreate<T, 4>(t_name);
        default:
            throw std::runtime_error("Attribute size not supported!");
        }
    }

    inline Attribute* findOrCreate(const std::string& t_name, const std::string& t_type) {
        if (t_type == "f" || t_type == "float")
            return findOrCreate<float, 1>(t_name);
        if (t_type == "v" || t_type == "vector")
            return findOrCreate<float, 3>(t_name);

        throw std::runtime_error("Attribute type not supported!");
        return nullptr;
    }

    inline Attribute* rename(const std::string& t_name, const std::string& t_newName) {
        auto attr = find(t_name);
        if (!attr)
            return nullptr;

        attr->setName(t_newName);
        auto node = m_map.extract(t_name);
        node.key() = t_newName;

        m_map.insert(std::move(node));
        return attr;
    };

    [[nodiscard]] const Attribute* get(const size_t t_index) const;
    [[nodiscard]] Attribute* get(const size_t t_index);

    void resize(const size_t t_size);
    [[nodiscard]] inline size_t size() const noexcept { return m_size; }
    [[nodiscard]] inline size_t count() const noexcept { return m_attributes.size(); }

    inline void clear() {
        m_map.clear();
        m_attributes.clear();
    }

  private:
    size_t m_size = 0;
    std::vector<std::unique_ptr<Attribute>> m_attributes;
    std::unordered_map<std::string, int> m_map;

  private:
    void copy(const AttributeSet& t_other);
};

} // namespace euclide