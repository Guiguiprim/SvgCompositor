#include "movable_svg_item.hpp"

namespace SvgCompositor
{

MovableSvgItem::MovableSvgItem(const QString& filename, QGraphicsItem *parent)
  : QGraphicsSvgItem(filename, parent)
  , _isBeingMoved(false)
{
  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void MovableSvgItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  QGraphicsSvgItem::mouseMoveEvent(event);
  _isBeingMoved = true;
}

void MovableSvgItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
  QGraphicsSvgItem::mouseReleaseEvent(event);
  if(_isBeingMoved)
  {
    _isBeingMoved = false;
    emit moveEnded();
  }
}

} // namespace SvgCompositor
