#ifndef MOVE_STRUCT_HPP
#define MOVE_STRUCT_HPP

#include <QList>

namespace SvgCompositor
{

struct ItemMove
{
  int index;
  qreal dx;
  qreal dy;
};

typedef QList<ItemMove> ItemsMove;

//qRegisterMetaType<ItemMove>("ItemMove");
//qRegisterMetaType<ItemsMove>("ItemsMove");

} // namespace SvgCompositor

#endif // MOVE_STRUCT_HPP
