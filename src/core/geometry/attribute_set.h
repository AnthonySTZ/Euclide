#pragma once

#include "attribute.h"

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
    Attribute* findOrCreate(const std::string& t_name) {
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

    [[nodiscard]] const Attribute* get(const size_t t_index) const;
    [[nodiscard]] Attribute* get(const size_t t_index);

    void resize(const size_t t_size);
    [[nodiscard]] inline size_t size() const noexcept { return m_size; }
    [[nodiscard]] inline size_t count() const noexcept { return m_attributes.size(); }

  private:
    size_t m_size = 0;
    std::vector<std::unique_ptr<Attribute>> m_attributes;
    std::unordered_map<std::string, int> m_map;

  private:
    void copy(const AttributeSet& t_other);
};

} // namespace euclide