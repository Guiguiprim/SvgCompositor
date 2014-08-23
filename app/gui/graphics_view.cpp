#include "graphics_view.hpp"

#include "graphics_item.hpp"

namespace SvgCompositor
{

GraphicsView::GraphicsView(QWidget *parent)
  :  QGraphicsView(parent)
  , _scene(new QGraphicsScene(this))
  , _item(NULL)
{
  _scene->setBackgroundBrush(Qt::gray);
  this->setScene(_scene);
}

void GraphicsView::setItem(GraphicsItem* item)
{
  if(_item)
  {
    disconnect(_item, SIGNAL(sizeChanged(int)),
               this, SLOT(onComposeSizeChanged()));
    _scene->removeItem(_item);
  }
  _item = item;
  if(_item)
  {
    connect(_item, SIGNAL(sizeChanged(int)),
            SLOT(onComposeSizeChanged()));
    _scene->addItem(_item);
    xRefitItem();
  }
}

GraphicsItem* GraphicsView::item() const
{
  return _item;
}

void GraphicsView::onComposeSizeChanged()
{
  xRefitItem();
}

void GraphicsView::resizeEvent(QResizeEvent* event)
{
  xRefitItem();
  QGraphicsView::resizeEvent(event);
}

void GraphicsView::showEvent(QShowEvent* event)
{
  xRefitItem();
  QGraphicsView::showEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent* event)
{
  QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mousePressEvent(QMouseEvent* event)
{
  QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
  QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::xRefitItem()
{
  if(_item)
  {
    _scene->setSceneRect(_item->rect());
    this->fitInView(_item->globalRect(), Qt::KeepAspectRatio);
  }
}

} // namespace SvgCompositor
