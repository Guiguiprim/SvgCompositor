#ifndef COMPOSITION_MOVABLE_SVG_ITEM_HPP
#define COMPOSITION_MOVABLE_SVG_ITEM_HPP

#include <QGraphicsSvgItem>

namespace SvgCompositor
{

class MovableSvgItem : public QGraphicsSvgItem
{
  Q_OBJECT
public:
  explicit MovableSvgItem(const QString& filename, QGraphicsItem *parent = 0);

signals:
  void moveEnded();

public slots:

protected:
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
  bool _isBeingMoved;
};

} // namespace SvgCompositor

#endif // COMPOSITION_MOVABLE_SVG_ITEM_HPP
