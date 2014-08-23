#ifndef ICON_HPP
#define ICON_HPP

#include <QIcon>

namespace SvgCompositor
{

class Icon
{
public:
  static QIcon& buildIcon(const QString& name);
};

} // namespace SvgCompositor

#endif // ICON_HPP
