#ifndef COMPOSITION_GRAPHICS_ITEM_HPP
#define COMPOSITION_GRAPHICS_ITEM_HPP

#include <QObject>
#include <QGraphicsRectItem>

#include <QDomElement>
#include <QMap>
#include <QRectF>
#include <QSize>
#include <QString>
#include <QVector>

#include "misc/move_struct.hpp"

class QGraphicsScene;
class QGraphicsSvgItem;
class QGraphicsRectItem;

namespace SvgCompositor
{
class MovableSvgItem;

class GraphicsItem : public QObject, public QGraphicsRectItem
{
  Q_OBJECT
public:
  GraphicsItem();

  QRectF globalRect() const;
  QSizeF size() const;

  int indexOf(QGraphicsItem* item) const;
  int indexSelection();

public slots:
  void setBackground(const QString& file);
  void addElement(const QString& file, int index = -1);
  void setSize(int size);
  void removeElement(int index);
  void setElementScale(int index, qreal scale);
  void setElementDx(int index, qreal dx);
  void setElementDy(int index, qreal dy);
  void setElementPos(int index, qreal dx, qreal dy);
  void lowerElement(int index);
  void raiseElement(int index);

private slots:
  void xOnElementMoved();

signals:
  void elementsMoved(ItemsMove itemsMove);
  void sizeChanged(int size);
  void selectionIndexChanged(int index);

private:
  bool xIndexValid(int index) const;

  void xUpdateZValues();

private:
  QGraphicsSvgItem* _transparent;
  QGraphicsSvgItem* _background;
  QVector<MovableSvgItem*> _elements;
};

} // namespace SvgCompositor

#endif // COMPOSITION_GRAPHICS_ITEM_HPP
