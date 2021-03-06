#ifndef PROJECT_MODEL_ENUM_HPP
#define PROJECT_MODEL_ENUM_HPP

#include <Qt>

namespace SvgCompositor {

enum Role
{
  TypeRole = Qt::UserRole + 1,
  OpenRole,
  BackgroundRole
};

enum ItemType
{
  AssemblyType,
  BackgroundType,
  ElementType
};

} // namespace SvgCompositor

#endif // PROJECT_MODEL_ENUM_HPP
