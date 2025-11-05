#pragma once

#include "utils/observer.h"

namespace butter {

class NodeFieldBase {
public:
    virtual ~NodeFieldBase() = default;

    Observer<> onValueChanged;
};

template <typename T>
class NodeField : public NodeFieldBase {

public:
    NodeField(const T& t_defaultValue) : m_value(t_defaultValue) {}

    const T& getValue() const noexcept { return m_value; }
    void setValue(const T& t_value){
        if (t_value == m_value) return;
        m_value = t_value;
        onValueChanged.notify();
    }

private:
    T m_value;
};

}