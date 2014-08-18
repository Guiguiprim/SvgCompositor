#ifndef ICON_HPP
#define ICON_HPP

#include <QIcon>

class Icon
{
public:
  static QIcon& buildIcon(const QString& name);
};

#endif // ICON_HPP
