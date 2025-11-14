#pragma once

#include "utils/observer.h"
#include "node_field_visitor.h"

#include <optional>

namespace butter {
    
struct NodeFieldMetadata {
    std::optional<std::string> displayName;
    std::optional<float> min;
    std::optional<float> max;
    std::optional<float> step;

    bool hidden = false;
};

class NodeFieldBase {
public:
    virtual ~NodeFieldBase() = default;
    virtual void accept(const std::string& t_name, NodeFieldVisitor& t_visitor) = 0;
    void setMetadata(const NodeFieldMetadata& t_meta) { m_metadata = t_meta; }
    const NodeFieldMetadata& metadata() const noexcept { return m_metadata; }


    Observer<> onValueChanged;

private:
    NodeFieldMetadata m_metadata;
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

    void accept(const std::string& t_name, NodeFieldVisitor& t_visitor) override {
        t_visitor.visit(t_name, *this);
    }

protected:
    T m_value;
};


}