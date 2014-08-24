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

QIcon& Icon::create() { return buildIcon("file"); }
QIcon& Icon::open() { return buildIcon("folder-open"); }
QIcon& Icon::save() { return buildIcon("save"); }
QIcon& Icon::saveAll() { return buildIcon("save_all"); }
QIcon& Icon::close() { return buildIcon("close"); }
QIcon& Icon::exit() { return buildIcon("exit"); }

QIcon& Icon::undo() { return buildIcon("undo"); }
QIcon& Icon::redo() { return buildIcon("redo"); }

QIcon& Icon::image() { return buildIcon("image"); }
QIcon& Icon::images() { return buildIcon("images"); }
QIcon& Icon::add() { return buildIcon("add"); }
QIcon& Icon::remove() { return buildIcon("remove"); }

} // namespace SvgCompositor
