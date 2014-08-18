#include "view_controller.hpp"

#include <svg_compose/svg_assembly.hpp>

#include "../gui/graphics_item.hpp"
#include "../gui/graphics_view.hpp"

namespace SvgCompositor
{

ViewController::ViewController(GraphicsView* view, QObject *parent)
  : QObject(parent)
  , _view(view)
  , _item(NULL)
  , _assembly(NULL)
{
  connect(_view->scene(), SIGNAL(selectionChanged()),
          SLOT(xOnSelectionChanged()));
}

void ViewController::setAssembly(SvgCompose::SvgAssembly* assembly)
{
  if(_assembly)
  {
    delete _item;
    _item = NULL;
  }
  _assembly = assembly;
  if(_assembly)
  {
    xCreateItem();
    xConnectDescription();
  }
}

void ViewController::xConnectDescription()
{
  connect(_assembly, SIGNAL(backgroundChanged(QString)),
          _item, SLOT(setBackground(QString)));
  connect(_assembly, SIGNAL(elementAdded(QString,int)),
          _item, SLOT(addElement(QString,int)));
  connect(_assembly, SIGNAL(sizeChanged(int)),
          _item, SLOT(setSize(int)));
  connect(_assembly, SIGNAL(elementRemoved(int)),
          _item, SLOT(removeElement(int)));
  connect(_assembly, SIGNAL(elementScaleChanged(int,qreal)),
          _item, SLOT(setElementScale(int,qreal)));
  connect(_assembly, SIGNAL(elementDxChanged(int,qreal)),
          _item, SLOT(setElementDx(int,qreal)));
  connect(_assembly, SIGNAL(elementDyChanged(int,qreal)),
          _item, SLOT(setElementDy(int,qreal)));
  connect(_assembly, SIGNAL(elementPosChanged(int,qreal,qreal)),
          _item, SLOT(setElementPos(int,qreal,qreal)));
  connect(_assembly, SIGNAL(elementLowered(int)),
          _item, SLOT(lowerElement(int)));
  connect(_assembly, SIGNAL(elementRaised(int)),
          _item, SLOT(raiseElement(int)));

  connect(_item, SIGNAL(elementsMoved(ItemsMove)),
          SIGNAL(elementsMoved(ItemsMove)));
  connect(_item, SIGNAL(selectionIndexChanged(int)),
          SIGNAL(selectionChanged(int)));
}

void ViewController::xCreateItem()
{
  _item = new GraphicsItem;
  _item->setBackground(_assembly->background());
  int index = 0;
  Q_FOREACH(SvgCompose::SvgAssemblyElement elem, _assembly->elements())
  {
    _item->addElement(elem.file);
    _item->setElementPos(index, elem.dx, elem.dy);
    _item->setElementScale(index, elem.scale);
    ++index;
  }
  _view->setItem(_item);
}

void ViewController::xOnSelectionChanged()
{
  if(_item)
    emit selectionChanged(_item->indexSelection());
}

} // namespace Composition
