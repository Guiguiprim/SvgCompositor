#ifndef ICON_HPP
#define ICON_HPP

#include <QIcon>

namespace SvgCompositor
{

class Icon
{
public:
  static QIcon& buildIcon(const QString& name);

  static QIcon& create();
  static QIcon& open();
  static QIcon& save();
  static QIcon& saveAll();
  static QIcon& close();
  static QIcon& exit();

  static QIcon& undo();
  static QIcon& redo();

  static QIcon& image();
  static QIcon& images();
  static QIcon& add();
  static QIcon& remove();
};

} // namespace SvgCompositor

#endif // ICON_HPP
