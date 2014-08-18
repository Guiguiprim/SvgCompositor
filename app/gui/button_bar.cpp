#include "button_bar.hpp"

#include <QSignalMapper>

#include "ui_button_bar.h"

namespace SvgCompositor
{

ButtonBar::ButtonBar(QWidget *parent)
  : QWidget(parent)
  , _ui(new Ui::ButtonBar)
{
  _ui->setupUi(this);

  QSignalMapper* map = new QSignalMapper(this);

  // assembly action
  connect(_ui->backgroundButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->backgroundButton, SetBackground);

  connect(_ui->noBackButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->noBackButton, RemoveBackground);

  connect(_ui->addButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->addButton, AddElement);

  connect(_ui->sizeUpButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->sizeUpButton, SizeIncrease);

  connect(_ui->sizeDownButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->sizeDownButton, SizeDecrease);

  // assembly element action
  connect(_ui->removeButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->removeButton, Remove);

  connect(_ui->downLayoutButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->downLayoutButton, Lower);

  connect(_ui->upLayoutButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->upLayoutButton, Raise);

  connect(_ui->centerHButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->centerHButton, HCenter);

  connect(_ui->centerVButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->centerVButton, VCenter);

  connect(_ui->zoomInButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->zoomInButton, ScaleIncrease);

  connect(_ui->zoomOutButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->zoomOutButton, ScaleDecrease);

  connect(_ui->upButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->upButton, MoveUp);

  connect(_ui->downButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->downButton, MoveDown);

  connect(_ui->leftButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->leftButton, MoveLeft);

  connect(_ui->rightButton, SIGNAL(clicked()), map, SLOT(map()));
  map->setMapping(_ui->rightButton, MoveRight);

  connect(map, SIGNAL(mapped(int)), SLOT(xOnActionTriggered(int)));

  connect(_ui->nameEdit, SIGNAL(editingFinished()),
          SLOT(xOnNameEdited()));

  setButtonsSize(20);
}

ButtonBar::~ButtonBar()
{
  delete _ui;
}

QString ButtonBar::name() const
{
  return _ui->nameEdit->text();
}

qreal ButtonBar::size() const
{
  return _ui->sizeSpin->value();
}

qreal ButtonBar::scale() const
{
  return _ui->scaleSpin->value();
}

qreal ButtonBar::dx() const
{
  return _ui->xPosSpin->value();
}

qreal ButtonBar::dy() const
{
  return _ui->yPosSpin->value();
}

void ButtonBar::setName(const QString& name)
{
  _ui->nameEdit->setText(name);
}

void ButtonBar::setNameValid(bool valid)
{
  if(!valid)
  {
    _ui->nameEdit->setStyleSheet("color:red");
    _ui->nameEdit->setToolTip("Name already used in this project");
  }
  else
  {
    _ui->nameEdit->setStyleSheet("");
    _ui->nameEdit->setToolTip("");
  }
}

void ButtonBar::setSizeValue(int size)
{
  _ui->sizeSpin->setValue(size);
}

void ButtonBar::setScaleValue(qreal scale)
{
  _ui->scaleSpin->setValue(scale);
}

void ButtonBar::setXPosValue(qreal x)
{
  _ui->xPosSpin->setValue(x);
}

void ButtonBar::setYPosValue(qreal y)
{
  _ui->yPosSpin->setValue(y);
}

void ButtonBar::setSelectionExist(bool exist)
{
  _ui->removeButton->setEnabled(exist);
  _ui->leftButton->setEnabled(exist);
  _ui->xPosSpin->setEnabled(exist);
  _ui->rightButton->setEnabled(exist);
  _ui->upButton->setEnabled(exist);
  _ui->yPosSpin->setEnabled(exist);
  _ui->downButton->setEnabled(exist);
  _ui->zoomInButton->setEnabled(exist);
  _ui->scaleSpin->setEnabled(exist);
  _ui->zoomOutButton->setEnabled(exist);
  _ui->upLayoutButton->setEnabled(exist);
  _ui->downLayoutButton->setEnabled(exist);
}

void ButtonBar::setSelectionCentrable(bool centrable)
{
  _ui->centerHButton->setEnabled(centrable);
  _ui->centerVButton->setEnabled(centrable);
}

void ButtonBar::setBackgroundExist(bool exist)
{
  _ui->noBackButton->setEnabled(exist);
}

void ButtonBar::setButtonsSize(int size)
{
  _ui->centerVButton->setIconSize(QSize(size, size));
  _ui->upLayoutButton->setIconSize(QSize(size, size));
  _ui->downLayoutButton->setIconSize(QSize(size, size));
  _ui->addButton->setIconSize(QSize(size, size));
  _ui->leftButton->setIconSize(QSize(size, size));
  _ui->rightButton->setIconSize(QSize(size, size));
  _ui->removeButton->setIconSize(QSize(size, size));
  _ui->zoomOutButton->setIconSize(QSize(size, size));
  _ui->zoomInButton->setIconSize(QSize(size, size));
  _ui->centerHButton->setIconSize(QSize(size, size));
  _ui->sizeDownButton->setIconSize(QSize(size, size));
  _ui->sizeUpButton->setIconSize(QSize(size, size));
  _ui->upButton->setIconSize(QSize(size, size));
  _ui->downButton->setIconSize(QSize(size, size));
  _ui->noBackButton->setIconSize(QSize(size, size));
  _ui->backgroundButton->setIconSize(QSize(size, size));
}

void ButtonBar::xOnActionTriggered(int action)
{
  emit actionTriggered((Action)action);
}

void ButtonBar::xOnNameEdited()
{
  emit nameChanged(_ui->nameEdit->text());
}

} // namespace Composition
