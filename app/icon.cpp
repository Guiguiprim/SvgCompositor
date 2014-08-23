#include "icon.hpp"

#include <QMap>

namespace SvgCompositor
{

QIcon& Icon::buildIcon(const QString& name)
{
  static QMap<QString, QIcon> icons;
  QMap<QString, QIcon>::iterator it = icons.find(name);
  if(it != icons.end())
    return *it;

  QIcon icon;
  icon.addFile(":/media/icons/" + name + ".svg");

  icons[name] = icon;
  return icons[name];
}

} // namespace SvgCompositor
