#pragma once

namespace butter {

template <typename T>
class NodeField {

public:
    NodeField(const T& t_defaultValue) : m_value(t_defaultValue) {}
    virtual ~NodeField() = default;

    const T& getValue() const noexcept { return m_value; }
    void setValue(const T& t_value){
        m_value = t_value;
        markDirty();
    }
    
    void markDirty() noexcept { m_isDirty = true; }
    void unmarkDirty() noexcept { m_isDirty = false; }
    bool isDirty() const noexcept { return m_isDirty; }

private:
    T m_value;
    bool m_isDirty = true;
};

}