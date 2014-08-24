#include "assembly_command.hpp"

#include <svg_compose/svg_assembly.hpp>

namespace SvgCompositor
{

namespace
{
template<typename T>
const T* mergeWithBase(T* c1, const QUndoCommand* o)
{
  const T* c2 = dynamic_cast<const T*>(o);
  if(!c2)
    return NULL;

  if(c1->assembly() != c2->assembly() ||
     c1->final() || c1->final() || c1 == c2)
    return NULL;

  return c2;
}

template<typename T>
const T* mergeWithIndex(T* c1, const QUndoCommand* o)
{
  const T* c2 = dynamic_cast<const T*>(o);
  if(!c2)
    return NULL;

  if(c1->assembly() != c2->assembly() ||
     c1->index() != c2->index() ||
     c1->final() || c1->final() || c1 == c2)
    return NULL;

  return c2;
}
} // namespace


// :::::::: SvgAssemblyCommand ::::::::::
SvgAssemblyCommandBase::SvgAssemblyCommandBase(
    SvgCompose::SvgAssembly* assembly,
    bool final,
    QUndoCommand* parent)
  : QUndoCommand(parent)
  , _assembly(assembly)
  , _final(final)
{}

// :::::::: SvgAssemblyCommandIndex ::::::::::
SvgAssemblyCommandIndex::SvgAssemblyCommandIndex(
    SvgCompose::SvgAssembly* assembly,
    int index,
    bool final,
    QUndoCommand* parent)
  : SvgAssemblyCommandBase(assembly, final, parent)
  , _index(index)
{}

namespace SvgAssemblyCommand
{

// :::::::: SetName ::::::::::
SetName::SetName(
    SvgCompose::SvgAssembly* assembly,
    QString name,
    bool final,
    QUndoCommand* parent)
  : SvgAssemblyCommandBase(assembly, final, parent)
  , _oldName(assembly->name())
  , _name(name)
{
  setText("Set name");
}

bool SetName::mergeWith(const QUndoCommand* other)
{
  const SetName* cmd = mergeWithBase(this, other);
  if(!cmd)
    return false;
  _name = cmd->_name;
  return true;
}

void SetName::redo()
{
  _assembly->setName(_name);
}

void SetName::undo()
{
  _assembly->setName(_oldName);
}

// :::::::: SetBackground ::::::::::
SetBackground::SetBackground(
    SvgCompose::SvgAssembly* assembly,
    QString background,
    QUndoCommand* parent)
  : SvgAssemblyCommandBase(assembly, true, parent)
  , _oldBackground(assembly->background())
  , _background(background)
{
  setText("Set background");
}

bool SetBackground::mergeWith(const QUndoCommand* other)
{
  return false;
}

void SetBackground::redo()
{
  _assembly->setBackground(_background);
}

void SetBackground::undo()
{
  _assembly->setBackground(_oldBackground);
}

// :::::::: RemoveBackground ::::::::::
RemoveBackground::RemoveBackground(
    SvgCompose::SvgAssembly* assembly,
    QUndoCommand* parent)
  : SvgAssemblyCommandBase(assembly, false, parent)
  , _oldBackground(assembly->background())
{
  setText("Remove background");
}

bool RemoveBackground::mergeWith(const QUndoCommand* other)
{
  const RemoveBackground* cmd = mergeWithBase(this, other);
  if(!cmd)
    return false;
  return true;
}

void RemoveBackground::redo()
{
  _assembly->setBackground("");
}

void RemoveBackground::undo()
{
  _assembly->setBackground(_oldBackground);
}

// :::::::: AddElement ::::::::::
AddElement::AddElement(
    SvgCompose::SvgAssembly* assembly,
    QString file,
    QUndoCommand* parent)
  : SvgAssemblyCommandBase(assembly, true, parent)
  , _addedIndex(0)
  , _file(file)
{
  setText("Add element");
}

bool AddElement::mergeWith(const QUndoCommand* other)
{
  return false;
}

void AddElement::redo()
{
  _assembly->addElement(_file);
  _addedIndex = _assembly->elementsCount() - 1;
}

void AddElement::undo()
{
  _assembly->removeElement(_addedIndex);
}

// :::::::: SizeChanged ::::::::::
SizeChanged::SizeChanged(
    SvgCompose::SvgAssembly* assembly,
    SvgCompose::SvgAssemblySize size,
    bool final,
    QUndoCommand* parent)
  : SvgAssemblyCommandBase(assembly, final, parent)
  , _oldSize(assembly->size())
  , _size(size)
{
  setText("Changed size");
}

bool SizeChanged::mergeWith(const QUndoCommand* other)
{
  const SizeChanged* cmd = mergeWithBase(this, other);
  if(!cmd)
    return false;
  _size = cmd->_size;
  return true;
}

void SizeChanged::redo()
{
  _assembly->setSize(_size);
}

void SizeChanged::undo()
{
  _assembly->setSize(_oldSize);
}

// :::::::: Remove ::::::::::
Remove::Remove(
    SvgCompose::SvgAssembly* assembly,
    int index,
    QUndoCommand* parent)
  : SvgAssemblyCommandIndex(assembly, index, true, parent)
  , _oldElement(assembly->element(index))
{
  setText("Remove element");
}

bool Remove::mergeWith(const QUndoCommand* other)
{
  return false;
}

void Remove::redo()
{
  _assembly->removeElement(_index);
}

void Remove::undo()
{
  _assembly->addElement(_oldElement.file, _index);
  _assembly->setElementPos(_index, _oldElement.dx, _oldElement.dy);
  _assembly->setElementScale(_index, _oldElement.scale);
}

// :::::::: Lower ::::::::::
Lower::Lower(
    SvgCompose::SvgAssembly* assembly,
    int index,
    QUndoCommand* parent)
  : SvgAssemblyCommandIndex(assembly, index, true, parent)
  , _oldIndex(index - 1)
{
  setText("Lower element");
}

bool Lower::mergeWith(const QUndoCommand* other)
{
  return false;
}

void Lower::redo()
{
  _assembly->lowerElement(_index);
}

void Lower::undo()
{
  // if _index == 0 => lower do nothing
  // and _oldIndex == -1 so raise do nothing
  _assembly->raiseElement(_oldIndex);
}

// :::::::: Raise ::::::::::
Raise::Raise(
    SvgCompose::SvgAssembly* assembly,
    int index,
    QUndoCommand* parent)
  : SvgAssemblyCommandIndex(assembly, index, true, parent)
  , _oldIndex(index + 1)
{
  setText("Raise element");
}

bool Raise::mergeWith(const QUndoCommand* other)
{
  return false;
}

void Raise::redo()
{
  _assembly->raiseElement(_index);
}

void Raise::undo()
{
  // if _index == _assembly.count() - 1 => raise do nothing
  // and _oldIndex == _assembly.count() so lower do nothing
  _assembly->lowerElement(_oldIndex);
}

// :::::::: HCenter ::::::::::
HCenter::HCenter(
    SvgCompose::SvgAssembly* assembly,
    int index,
    QUndoCommand* parent)
  : SvgAssemblyCommandIndex(assembly, index, false, parent)
  , _oldDx(assembly->element(index).dx)
{
  setText("Horizontal center");
}

bool HCenter::mergeWith(const QUndoCommand* other)
{
  const HCenter* cmd = mergeWithIndex(this, other);
  if(!cmd)
    return false;
  return true;
}

void HCenter::redo()
{
  _assembly->setElementHCenter(_index);
}

void HCenter::undo()
{
  _assembly->setElementDx(_index, _oldDx);
}

// :::::::: VCenter ::::::::::
VCenter::VCenter(
    SvgCompose::SvgAssembly* assembly,
    int index,
    QUndoCommand* parent)
  : SvgAssemblyCommandIndex(assembly, index, false, parent)
  , _oldDy(assembly->element(index).dy)
{
  setText("Vertical center");
}

bool VCenter::mergeWith(const QUndoCommand* other)
{
  const VCenter* cmd = mergeWithIndex(this, other);
  if(!cmd)
    return false;
  return true;
}

void VCenter::redo()
{
  _assembly->setElementVCenter(_index);
}

void VCenter::undo()
{
  _assembly->setElementDy(_index, _oldDy);
}

// :::::::: ScaleChanged ::::::::::
ScaleChanged::ScaleChanged(
    SvgCompose::SvgAssembly* assembly,
    int index,
    qreal scale,
    bool final,
    QUndoCommand* parent)
  : SvgAssemblyCommandIndex(assembly, index, final, parent)
  , _oldScale(assembly->element(index).scale)
  , _scale(scale)
{
  setText("Change scale");
}

bool ScaleChanged::mergeWith(const QUndoCommand* other)
{
  const ScaleChanged* cmd = mergeWithIndex(this, other);
  if(!cmd)
    return false;
  _scale = cmd->_scale;
  return true;
}

void ScaleChanged::redo()
{
  _assembly->setElementScale(_index, _scale);
}

void ScaleChanged::undo()
{
  _assembly->setElementScale(_index, _oldScale);
}

// :::::::: MoveDy ::::::::::
MoveDy::MoveDy(
    SvgCompose::SvgAssembly* assembly,
    int index,
    qreal dy,
    bool final,
    QUndoCommand* parent)
  : SvgAssemblyCommandIndex(assembly, index, final, parent)
  , _oldDy(assembly->element(index).dy)
  , _dy(dy)
{
  setText("Move dy");
}

bool MoveDy::mergeWith(const QUndoCommand* other)
{
  const MoveDy* cmd = mergeWithIndex(this, other);
  if(!cmd)
    return false;
  _dy = cmd->_dy;
  return true;
}

void MoveDy::redo()
{
  _assembly->setElementDy(_index, _dy);
}

void MoveDy::undo()
{
  _assembly->setElementDy(_index, _oldDy);
}

// :::::::: MoveDx ::::::::::
MoveDx::MoveDx(
    SvgCompose::SvgAssembly* assembly,
    int index,
    qreal dx,
    bool final,
    QUndoCommand* parent)
  : SvgAssemblyCommandIndex(assembly, index, final, parent)
  , _oldDx(assembly->element(index).dx)
  , _dx(dx)
{
  setText("Move dx");
}

bool MoveDx::mergeWith(const QUndoCommand* other)
{
  const MoveDx* cmd = mergeWithIndex(this, other);
  if(!cmd)
    return false;
  _dx = cmd->_dx;
  return true;
}

void MoveDx::redo()
{
  _assembly->setElementDx(_index, _dx);
}

void MoveDx::undo()
{
  _assembly->setElementDx(_index, _oldDx);
}

// :::::::: SetPos ::::::::::
SetPos::SetPos(
    SvgCompose::SvgAssembly* assembly,
    int index,
    qreal dx,
    qreal dy,
    bool final,
    QUndoCommand* parent)
  : SvgAssemblyCommandIndex(assembly, index, final, parent)
  , _oldDx(assembly->element(index).dx)
  , _oldDy(assembly->element(index).dy)
  , _dx(dx)
  , _dy(dy)
{
  setText("Move");
}

bool SetPos::mergeWith(const QUndoCommand* other)
{
  const SetPos* cmd = mergeWithIndex(this, other);
  if(!cmd)
    return false;
  _dx = cmd->_dx;
  _dy = cmd->_dy;
  return true;
}

void SetPos::redo()
{
  _assembly->setElementPos(_index, _dx, _dy);
}

void SetPos::undo()
{
  _assembly->setElementPos(_index, _oldDx, _oldDy);
}

} // namespace SvgAssemblyCommand

} // namespace SvgCompositor
