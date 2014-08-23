#include "button_bar_controller.hpp"

#include <svg_compose/svg_assembly.hpp>

#include "gui/button_bar.hpp"

namespace SvgCompositor
{

ButtonBarController::ButtonBarController(ButtonBar* buttonBar, QObject *parent)
  : QObject(parent)
  , _buttonBar(buttonBar)
  , _assembly(NULL)
  , _sIndex(-1)
{
  connect(_buttonBar, SIGNAL(actionTriggered(Action)),
          SLOT(xOnActionTriggered(Action)));
  connect(_buttonBar, SIGNAL(nameChanged(QString)),
          SIGNAL(nameChanged(QString)));
}

void ButtonBarController::setAssembly(SvgCompose::SvgAssembly* assembly)
{
  if(_assembly)
  {
    _buttonBar->setEnabled(false);
    _buttonBar->setSelectionExist(false);
    _buttonBar->setBackgroundExist(false);
    _buttonBar->setSelectionCentrable(false);
    _buttonBar->setName("");
    tearDownConnection();
  }
  _assembly = assembly;
  if(_assembly)
  {
    _buttonBar->setEnabled(true);
    _buttonBar->setBackgroundExist(!_assembly->background().isEmpty());
    _buttonBar->setSizeValue(_assembly->size());
    _buttonBar->setName(_assembly->name());
    onSelectionChanged(-1);
    setUpConnection();
  }
}

void ButtonBarController::onSelectionChanged(int index)
{
  _sIndex = index;
  if(_assembly && index != -1)
  {
    SvgCompose::SvgAssemblyElement elem = _assembly->element(index);
    _buttonBar->setXPosValue(elem.dx);
    _buttonBar->setYPosValue(elem.dy);
    _buttonBar->setScaleValue(elem.scale);
    _buttonBar->setSelectionExist(true);
    _buttonBar->setSelectionCentrable(elem.validSize);
  }
  else
  {
    _buttonBar->setXPosValue(0.0);
    _buttonBar->setYPosValue(0.0);
    _buttonBar->setScaleValue(1.0);
    _buttonBar->setSelectionExist(false);
    _buttonBar->setSelectionCentrable(false);
  }
}

void ButtonBarController::xOnBackgroundChanged(const QString& background)
{
  _buttonBar->setBackgroundExist(!background.isEmpty());
}

void ButtonBarController::xOnSizeChanged(int size)
{
  _buttonBar->setSizeValue(size);
}

void ButtonBarController::xOnScaleChanged(int index, qreal scale)
{
  if(index == _sIndex && _sIndex != -1)
    _buttonBar->setScaleValue(scale);
}

void ButtonBarController::xOnDxChanged(int index, qreal dx)
{
  if(index == _sIndex && _sIndex != -1)
    _buttonBar->setXPosValue(dx);
}

void ButtonBarController::xOnDyChanged(int index, qreal dy)
{
  if(index == _sIndex && _sIndex != -1)
    _buttonBar->setYPosValue(dy);
}

void ButtonBarController::xOnPosChanged(int index, qreal dx, qreal dy)
{
  if(index == _sIndex && _sIndex != -1)
  {
    _buttonBar->setXPosValue(dx);
    _buttonBar->setYPosValue(dy);
  }
}

void ButtonBarController::xOnActionTriggered(Action action)
{
  emit actionTriggered(_sIndex, action);
}

void ButtonBarController::setUpConnection()
{
  connect(_assembly, SIGNAL(nameChanged(QString)),
          _buttonBar, SLOT(setName(QString)));
  connect(_assembly, SIGNAL(backgroundChanged(QString)),
          this, SLOT(xOnBackgroundChanged(QString)));
  connect(_assembly, SIGNAL(sizeChanged(int)),
          this, SLOT(xOnSizeChanged(int)));
  connect(_assembly, SIGNAL(elementScaleChanged(int,qreal)),
          this, SLOT(xOnScaleChanged(int,qreal)));
  connect(_assembly, SIGNAL(elementDxChanged(int,qreal)),
          this, SLOT(xOnDxChanged(int,qreal)));
  connect(_assembly, SIGNAL(elementDyChanged(int,qreal)),
          this, SLOT(xOnDyChanged(int,qreal)));
  connect(_assembly, SIGNAL(elementPosChanged(int,qreal,qreal)),
          this, SLOT(xOnPosChanged(int,qreal,qreal)));
}

void ButtonBarController::tearDownConnection()
{
  disconnect(_assembly, SIGNAL(nameChanged(QString)),
             _buttonBar, SLOT(setName(QString)));
  disconnect(_assembly, SIGNAL(backgroundChanged(QString)),
             this, SLOT(xOnBackgroundChanged(QString)));
  disconnect(_assembly, SIGNAL(sizeChanged(int)),
             this, SLOT(xOnSizeChanged(int)));
  disconnect(_assembly, SIGNAL(elementScaleChanged(int,qreal)),
             this, SLOT(xOnScaleChanged(int,qreal)));
  disconnect(_assembly, SIGNAL(elementDxChanged(int,qreal)),
             this, SLOT(xOnDxChanged(int,qreal)));
  disconnect(_assembly, SIGNAL(elementDyChanged(int,qreal)),
             this, SLOT(xOnDyChanged(int,qreal)));
  disconnect(_assembly, SIGNAL(elementPosChanged(int,qreal,qreal)),
             this, SLOT(xOnPosChanged(int,qreal,qreal)));
}

} // namespace SvgCompositor
