#include "graphics_item.hpp"

#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QTextStream>

#include <QDebug>

#include "movable_svg_item.hpp"

namespace SvgCompositor
{

namespace
{
qreal k_defaultSize = 48;
} // namespace

GraphicsItem::GraphicsItem()
  : QObject()
  , QGraphicsRectItem()
  , _background(NULL)
  , _elements()
{
  this->setRect(0, 0, k_defaultSize, k_defaultSize);
  this->setZValue(-2);
  this->setBrush(Qt::white);
  this->setPen(Qt::NoPen);

  _transparent = new QGraphicsSvgItem(":/media/transparent.svg", this);
  _transparent->setZValue(-1);
  qreal sx = rect().width() / _transparent->renderer()->defaultSize().width();
  qreal sy = rect().height() / _transparent->renderer()->defaultSize().height();
  _transparent->setTransform(QTransform().scale(sx, sy));
}

QRectF GraphicsItem::globalRect() const
{
  return this->rect().adjusted(-3,-3,3,3);
}

QSizeF GraphicsItem::size() const
{
  return this->rect().size();
}

int GraphicsItem::indexOf(QGraphicsItem* item) const
{
  MovableSvgItem* svgItem = dynamic_cast<MovableSvgItem*>(item);
  if(svgItem)
    return _elements.indexOf(svgItem);
  return -1;
}

int GraphicsItem::indexSelection()
{
  QGraphicsScene* scene = this->scene();
  if(!scene)
    return -1;
  QList<QGraphicsItem*> items = scene->selectedItems();
  if(items.count() == 0 || items.count() > 1)
    return -1;
  return indexOf(items.at(0));
}

void GraphicsItem::setBackground(const QString& file)
{
  if(_background)
  {
    delete _background;
    _background = NULL;
  }

  if(!file.isEmpty())
  {
    _background = new QGraphicsSvgItem(file, this);
    _background->setZValue(0);
    qreal size = rect().width();
    qreal x = _background->renderer()->defaultSize().width();
    qreal y = _background->renderer()->defaultSize().height();
    qreal sx = size / x;
    qreal sy = size / y;
    qreal s = sx < sy ? sx : sy;
    qreal dx = (size - x * s) / 2;
    qreal dy = (size - y * s) / 2;
    _background->setScale(s);
    _background->setPos(dx,dy);
  }
}

void GraphicsItem::addElement(const QString& file, int index)
{
  MovableSvgItem* item = new MovableSvgItem(file, this);
  item->setZValue(2);
  item->setFlag(QGraphicsItem::ItemIsMovable, true);
  item->setFlag(QGraphicsItem::ItemIsSelectable, true);
  if(xIndexValid(index))
    _elements.insert(index,item);
  else
    _elements.push_back(item);
  xUpdateZValues();

  connect(item, SIGNAL(moveEnded()), SLOT(xOnElementMoved()));
}

void GraphicsItem::setSize(int sizeInt)
{
  qreal size = (qreal)sizeInt;
  if(rect().height() == size)
    return;

  this->setRect(0, 0, size, size);
  if(_background)
  {
    qreal x = _background->renderer()->defaultSize().width();
    qreal y = _background->renderer()->defaultSize().height();
    qreal sx = size / x;
    qreal sy = size / y;
    qreal s = sx < sy ? sx : sy;
    qreal dx = (size - x * s) / 2;
    qreal dy = (size - y * s) / 2;
    _background->setScale(s);
    _background->setPos(dx,dy);
  }

  {
    qreal sx = size / _transparent->renderer()->defaultSize().width();
    qreal sy = size / _transparent->renderer()->defaultSize().height();
    _transparent->setTransform(QTransform().scale(sx, sy));
  }

  emit sizeChanged(size);
}

void GraphicsItem::removeElement(int index)
{
  if(xIndexValid(index))
  {
    delete _elements.at(index);
    _elements.remove(index);
  }
}

void GraphicsItem::setElementScale(int index, qreal scale)
{
  if(xIndexValid(index))
    _elements[index]->setScale(scale);
}

void GraphicsItem::setElementDx(int index, qreal dx)
{
  if(xIndexValid(index))
    _elements[index]->setPos(dx, _elements.at(index)->pos().y());
}

void GraphicsItem::setElementDy(int index, qreal dy)
{
  if(xIndexValid(index))
    _elements[index]->setPos(_elements.at(index)->pos().x(), dy);
}

void GraphicsItem::setElementPos(int index, qreal dx, qreal dy)
{
  if(xIndexValid(index))
    _elements[index]->setPos(dx, dy);
}

void GraphicsItem::lowerElement(int index)
{
  if(xIndexValid(index) && index > 0)
  {
    MovableSvgItem* tmp = _elements[index];
    _elements[index] = _elements[index-1];
    _elements[index-1] = tmp;
    emit selectionIndexChanged(index-1);
    xUpdateZValues();
  }
}

void GraphicsItem::raiseElement(int index)
{
  if(xIndexValid(index) && index+1 < _elements.count())
  {
    MovableSvgItem* tmp = _elements[index];
    _elements[index] = _elements[index+1];
    _elements[index+1] = tmp;
    emit selectionIndexChanged(index+1);
    xUpdateZValues();
  }
}

void GraphicsItem::xOnElementMoved()
{
  QGraphicsScene* scene = this->scene();
  if(!scene)
    return;
  ItemsMove itemsMove;
  QList<QGraphicsItem*> items = scene->selectedItems();
  Q_FOREACH(QGraphicsItem* item, items)
  {
    int index = indexOf(item);
    if(index != -1)
    {
      ItemMove itemMove;
      itemMove.index = index;
      itemMove.dx = _elements.at(index)->pos().x();
      itemMove.dy = _elements.at(index)->pos().y();
      itemsMove << itemMove;
    }
  }
  if(itemsMove.count() > 0)
    emit elementsMoved(itemsMove);
}

bool GraphicsItem::xIndexValid(int index) const
{
  return index >= 0 && index < _elements.count();
}

void GraphicsItem::xUpdateZValues()
{
  for(int i=0; i<_elements.count(); ++i)
  {
    _elements[i]->setZValue(i+1);
  }
}

} // namespace SvgCompositor
