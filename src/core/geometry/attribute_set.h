#pragma once

#include "attribute.h"

namespace euclide {

class AttributeSet {
  public:
    AttributeSet() = default;
    ~AttributeSet() = default;

    AttributeSet(const AttributeSet& t_other);
    AttributeSet& operator=(const AttributeSet& t_other);

    [[nodiscard]] Attribute* find(const std::string& t_name);
    Attribute* findOrCreate(const std::string& t_name, const int t_attrSize, const AttributeType t_type);
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