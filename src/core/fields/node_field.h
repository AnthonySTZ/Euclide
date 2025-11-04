#pragma once

namespace butter {

class NodeFieldBase {
public:
    virtual ~NodeFieldBase() = default;
    void markDirty() noexcept { m_isDirty = true; }
    void unmarkDirty() noexcept { m_isDirty = false; }
    bool isDirty() const noexcept { return m_isDirty; }

private:
    bool m_isDirty = true;
};

template <typename T>
class NodeField : public NodeFieldBase {

public:
    NodeField(const T& t_defaultValue) : m_value(t_defaultValue) {}

    const T& getValue() const noexcept { return m_value; }
    void setValue(const T& t_value){
        m_value = t_value;
        markDirty();
    }

private:
    T m_value;
};

}