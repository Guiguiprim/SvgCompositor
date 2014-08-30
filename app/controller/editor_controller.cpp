#include "editor_controller.hpp"

#include <QFileDialog>
#include <QUndoStack>
#include <QWidget>

#include <svg_compose/svg_assembly.hpp>

#include "command/assembly_command.hpp"
#include "misc/last_dir.hpp"

namespace SvgCompositor
{

namespace
{
inline qreal tronc(qreal x)
{
  return (qreal)( (int)(x * 10) / 10.0);
}
inline qreal tronc_(qreal x)
{
  return (qreal)( (int)(x * 1000) / 1000.0);
}
} // namespace

EditorController::EditorController(QWidget *parent)
  : QObject(parent)
  , _parentWidget(parent)
  , _assembly(NULL)
  , _undoStack(new QUndoStack(this))
{
}

void EditorController::setAssembly(SvgCompose::SvgAssembly* assembly)
{
  _assembly = assembly;
}

QUndoStack* EditorController::undoStack() const
{
  return _undoStack;
}

void EditorController::onNameEdited(const QString& name)
{
  if(_assembly)
    _undoStack->push( new SvgAssemblyCommand::SetName(_assembly, name) );
}

void EditorController::onAction(int index, Action action)
{
  if(!_assembly)
    return;

  switch(action) // assembly action
  {
  case SetBackground:
  {
    QString absoluteFile = QFileDialog::getOpenFileName(
                             _parentWidget,"Background file",
                             LastDir::getBackgroundDir(), "SVG (*.svg)");
    if(absoluteFile.isEmpty())
      break;
    LastDir::saveBackgroundDir(QFileInfo(absoluteFile).absolutePath());
    _undoStack->push( new SvgAssemblyCommand::SetBackground(_assembly, absoluteFile) );
    break;
  }
  case RemoveBackground:
    _undoStack->push( new SvgAssemblyCommand::RemoveBackground(_assembly) );
    break;
  case AddElement:
  {
    QString absoluteFile = QFileDialog::getOpenFileName(
                             _parentWidget,"Add element",
                             LastDir::getLogoDir(), "SVG (*.svg)");
    if(absoluteFile.isEmpty())
      break;
    LastDir::saveLogoDir(QFileInfo(absoluteFile).absolutePath());
    _undoStack->push( new SvgAssemblyCommand::AddElement(_assembly, absoluteFile) );
    break;
  }
  case SizeIncrease:
    _undoStack->push( new SvgAssemblyCommand::SizeChanged(_assembly, increaseSize(_assembly->size())) );
    break;
  case SizeDecrease:
    _undoStack->push( new SvgAssemblyCommand::SizeChanged(_assembly, decreaseSize(_assembly->size())) );
    break;
  default:
    break;
  }

  if(index == -1 && index < _assembly->elementsCount())
    return;

  switch(action) // assembly element action
  {
  case Remove:
    _undoStack->push( new SvgAssemblyCommand::Remove(_assembly, index) );
    break;
  case Lower:
    _undoStack->push( new SvgAssemblyCommand::Lower(_assembly, index) );
    break;
  case Raise:
    _undoStack->push( new SvgAssemblyCommand::Raise(_assembly, index) );
    break;
  case HCenter:
    _undoStack->push( new SvgAssemblyCommand::HCenter(_assembly, index) );
    break;
  case VCenter:
    _undoStack->push( new SvgAssemblyCommand::VCenter(_assembly, index) );
    break;
  case ScaleIncrease:
    _undoStack->push( new SvgAssemblyCommand::ScaleChanged(_assembly, index,
                                                           tronc_(_assembly->element(index).scale * 1.1)) );
    break;
  case ScaleDecrease:
    _undoStack->push( new SvgAssemblyCommand::ScaleChanged(_assembly, index,
                                                           tronc_(_assembly->element(index).scale * 0.9)) );
    break;
  case MoveUp:
    _undoStack->push( new SvgAssemblyCommand::MoveDy(_assembly, index,
                                                     _assembly->element(index).dy - 0.5) );
    break;
  case MoveDown:
    _undoStack->push( new SvgAssemblyCommand::MoveDy(_assembly, index,
                                                     _assembly->element(index).dy + 0.5) );
    break;
  case MoveRight:
    _undoStack->push( new SvgAssemblyCommand::MoveDx(_assembly, index,
                                                     _assembly->element(index).dx + 0.5) );
    break;
  case MoveLeft:
    _undoStack->push( new SvgAssemblyCommand::MoveDx(_assembly, index,
                                                     _assembly->element(index).dx - 0.5) );
    break;
  default:
    break;
  }
}

void EditorController::onActionValue(int index, Action action, qreal value)
{
  if(!_assembly)
    return;

  if(index == -1 && index < _assembly->elementsCount())
    return;

  switch(action) // assembly element action
  {
  case ScaleChanged:
    _undoStack->push( new SvgAssemblyCommand::ScaleChanged(_assembly, index, value));
    break;
  case MoveDy:
    _undoStack->push( new SvgAssemblyCommand::MoveDy(_assembly, index, value));
    break;
  case MoveDx:
    _undoStack->push( new SvgAssemblyCommand::MoveDx(_assembly, index, value));
    break;
  default:
    break;
  }
}

void EditorController::onElementsMoved(ItemsMove itemsMove)
{
  if(_assembly && itemsMove.count() > 0)
  {
    QUndoCommand* cmd = new QUndoCommand;
    cmd->setText("Move");
    Q_FOREACH(ItemMove item, itemsMove)
    {
      new SvgAssemblyCommand::SetPos(_assembly, item.index,
                                     tronc(item.dx), tronc(item.dy),
                                     true, cmd);
    }
    _undoStack->push(cmd);
  }
}

} // namespace SvgCompositor
