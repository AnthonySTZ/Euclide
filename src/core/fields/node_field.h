#pragma once

#include "utils/observer.h"
#include "node_field_visitor.h"

#include <optional>
#include <string>
#include <vector>

namespace euclide {

/// @brief Metadata for a node field, used for UI display and constraints.
struct NodeFieldMetadata {
    std::optional<std::string> displayName; ///< Optional display name for UI.
    std::optional<float> min;               ///< Optional minimum value.
    std::optional<float> max;               ///< Optional maximum value.
    std::optional<float> step = 1.0f;       ///< Optional step for numeric fields.

    bool isMultiline = false;
    bool isCombo = false;                            ///< Whether this field is a combo box.
    std::optional<std::vector<std::string>> choices; ///< Optional choices for combo fields.

    std::function<bool()> shouldBeHidden = []() { return false; };
};

/// @brief Base class for a node field.
class NodeFieldBase {
  public:
    virtual ~NodeFieldBase() = default;

    /// @brief Accepts a visitor for rendering or editing the field.
    /// @param t_name Name of the field.
    /// @param t_visitor Visitor object.
    virtual void accept(const std::string& t_name, NodeFieldVisitor& t_visitor) = 0;

    /// @brief Sets the metadata for this field.
    /// @param t_meta Metadata structure.
    void setMetadata(const NodeFieldMetadata& t_meta) { m_metadata = t_meta; }

    /// @brief Returns the metadata for this field.
    /// @return Const reference to NodeFieldMetadata.
    const NodeFieldMetadata& metadata() const noexcept { return m_metadata; }

    /// @brief Notifies observers when the field value changes.
    Observer<> onValueChanged;

  private:
    NodeFieldMetadata m_metadata; ///< Metadata for this field
};

/// @brief Template class representing a typed node field.
template <typename T>
class NodeField : public NodeFieldBase {
  public:
    /// @brief Constructs a NodeField with a default value.
    /// @param t_defaultValue Initial value for the field.
    NodeField(const T& t_defaultValue) : m_value(t_defaultValue) {}

    /// @brief Returns the current value of the field.
    /// @return
    T getValue() const noexcept { return m_value; }

    /// @brief Sets the value of the field.
    /// @param t_value New value to set.
    /// @note Notifies observers if the value changes.
    void setValue(const T& t_value) {
        if (t_value == m_value)
            return;
        m_value = t_value;
        onValueChanged.notify();
    }

    /// @brief Accepts a visitor for rendering or editing.
    /// @param t_name Name of the field.
    /// @param t_visitor Visitor object.
    void accept(const std::string& t_name, NodeFieldVisitor& t_visitor) override { t_visitor.visit(t_name, *this); }

  protected:
    T m_value; ///< Current value of the field
};

} // namespace euclide
