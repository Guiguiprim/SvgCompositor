#ifndef ASSEMBLY_COMMAND_HPP
#define ASSEMBLY_COMMAND_HPP

#include <QUndoCommand>

#include <svg_compose/svg_assembly_element.hpp>
#include <svg_compose/svg_assembly_size.hpp>

#include "../controller/action.hpp"

namespace SvgCompose
{
class SvgAssembly;
} // namespace SvgCompose

namespace SvgCompositor
{

// :::::SvgAssemblyCommandBase ::::::::
class SvgAssemblyCommandBase : public QUndoCommand
{
public:
  SvgAssemblyCommandBase(
      SvgCompose::SvgAssembly* assembly,
      bool final = false,
      QUndoCommand* parent = NULL);

  SvgCompose::SvgAssembly* assembly() const { return _assembly; }
  bool final() const { return _final; }

protected:
  SvgCompose::SvgAssembly* _assembly;
  bool _final;
};

// :::::SvgAssemblyCommandIndex ::::::::
class SvgAssemblyCommandIndex : public SvgAssemblyCommandBase
{
public:
  SvgAssemblyCommandIndex(
      SvgCompose::SvgAssembly* assembly,
      int index,
      bool final = false,
      QUndoCommand* parent = NULL);

  int index() const { return _index; }

protected:
  int _index;
};

namespace SvgAssemblyCommand
{

// :::::::: SetName ::::::::::
class SetName : public SvgAssemblyCommandBase
{
public:
  SetName(
      SvgCompose::SvgAssembly* assembly,
      QString name,
      bool final = true,
      QUndoCommand* parent = NULL);

  virtual int id() const { return SvgCompositor::SetName; }
  virtual bool mergeWith(const QUndoCommand* other);
  virtual void redo();
  virtual void undo();

protected:
  QString _oldName;
  QString _name;
};

// :::::::: SetBackground ::::::::::
class SetBackground : public SvgAssemblyCommandBase
{
public:
  SetBackground(
      SvgCompose::SvgAssembly* assembly,
      QString background,
      QUndoCommand* parent = NULL);

  virtual bool mergeWith(const QUndoCommand* other);
  virtual void redo();
  virtual void undo();

protected:
  QString _oldBackground;
  QString _background;
};

// :::::::: RemoveBackground ::::::::::
class RemoveBackground : public SvgAssemblyCommandBase
{
public:
  RemoveBackground(
      SvgCompose::SvgAssembly* assembly,
      QUndoCommand* parent = NULL);

  virtual int id() const { return SvgCompositor::RemoveBackground; }
  virtual bool mergeWith(const QUndoCommand* other);
  virtual void redo();
  virtual void undo();

protected:
  QString _oldBackground;
};

// :::::::: AddElement ::::::::::
class AddElement : public SvgAssemblyCommandBase
{
public:
  AddElement(
      SvgCompose::SvgAssembly* assembly,
      QString file,
      QUndoCommand* parent = NULL);

  virtual bool mergeWith(const QUndoCommand* other);
  virtual void redo();
  virtual void undo();

protected:
  int _addedIndex;
  QString _file;
};

// :::::::: SizeChanged ::::::::::
class SizeChanged : public SvgAssemblyCommandBase
{
public:
  SizeChanged(
      SvgCompose::SvgAssembly* assembly,
      SvgCompose::SvgAssemblySize size,
      bool final = false,
      QUndoCommand* parent = NULL);

  virtual int id() const { return SvgCompositor::SizeIncrease; }
  virtual bool mergeWith(const QUndoCommand* other);
  virtual void redo();
  virtual void undo();

protected:
  SvgCompose::SvgAssemblySize _oldSize;
  SvgCompose::SvgAssemblySize _size;
};

// :::::::: Remove ::::::::::
class Remove : public SvgAssemblyCommandIndex
{
public:
  Remove(
      SvgCompose::SvgAssembly* assembly,
      int index,
      QUndoCommand* parent = NULL);

  virtual bool mergeWith(const QUndoCommand* other);
  virtual void redo();
  virtual void undo();

protected:
  SvgCompose::SvgAssemblyElement  _oldElement;
};

// :::::::: Lower ::::::::::
class Lower : public SvgAssemblyCommandIndex
{
public:
  Lower(
      SvgCompose::SvgAssembly* assembly,
      int index,
      QUndoCommand* parent = NULL);

  virtual bool mergeWith(const QUndoCommand* other);
  virtual void redo();
  virtual void undo();

protected:
  int _oldIndex;
};

// :::::::: Raise ::::::::::
class Raise : public SvgAssemblyCommandIndex
{
public:
  Raise(
      SvgCompose::SvgAssembly* assembly,
      int index,
      QUndoCommand* parent = NULL);

  virtual bool mergeWith(const QUndoCommand* other);
  virtual void redo();
  virtual void undo();

protected:
  int _oldIndex;
};

// :::::::: HCenter ::::::::::
class HCenter : public SvgAssemblyCommandIndex
{
public:
  HCenter(
      SvgCompose::SvgAssembly* assembly,
      int index,
      QUndoCommand* parent = NULL);

  virtual int id() const { return SvgCompositor::HCenter; }
  virtual bool mergeWith(const QUndoCommand* other);
  virtual void redo();
  virtual void undo();

protected:
  qreal _oldDx;
};

// :::::::: VCenter ::::::::::
class VCenter : public SvgAssemblyCommandIndex
{
public:
  VCenter(
      SvgCompose::SvgAssembly* assembly,
      int index,
      QUndoCommand* parent = NULL);

  virtual int id() const { return SvgCompositor::VCenter; }
  virtual bool mergeWith(const QUndoCommand* other);
  virtual void redo();
  virtual void undo();

protected:
  qreal _oldDy;
};

// :::::::: ScaleChanged ::::::::::
class ScaleChanged : public SvgAssemblyCommandIndex
{
public:
  ScaleChanged(
      SvgCompose::SvgAssembly* assembly,
      int index,
      qreal scale,
      bool final = false,
      QUndoCommand* parent = NULL);

  virtual int id() const { return SvgCompositor::ScaleIncrease; }
  virtual bool mergeWith(const QUndoCommand* other);
  virtual void redo();
  virtual void undo();

protected:
  qreal _oldScale;
  qreal _scale;
};

// :::::::: MoveDy ::::::::::
class MoveDy : public SvgAssemblyCommandIndex
{
public:
  MoveDy(
      SvgCompose::SvgAssembly* assembly,
      int index,
      qreal dy,
      bool final = false,
      QUndoCommand* parent = NULL);

  virtual int id() const { return SvgCompositor::MoveUp; }
  virtual bool mergeWith(const QUndoCommand* other);
  virtual void redo();
  virtual void undo();

protected:
  qreal _oldDy;
  qreal _dy;
};

// :::::::: MoveDx ::::::::::
class MoveDx : public SvgAssemblyCommandIndex
{
public:
  MoveDx(
      SvgCompose::SvgAssembly* assembly,
      int index,
      qreal dx,
      bool final = false,
      QUndoCommand* parent = NULL);

  virtual int id() const { return SvgCompositor::MoveLeft; }
  virtual bool mergeWith(const QUndoCommand* other);
  virtual void redo();
  virtual void undo();

protected:
  qreal _oldDx;
  qreal _dx;
};

// :::::::: SetPos ::::::::::
class SetPos : public SvgAssemblyCommandIndex
{
public:
  SetPos(
      SvgCompose::SvgAssembly* assembly,
      int index,
      qreal dx,
      qreal dy,
      bool final = false,
      QUndoCommand* parent = NULL);

  virtual int id() const { return SvgCompositor::SetPos; }
  virtual bool mergeWith(const QUndoCommand* other);
  virtual void redo();
  virtual void undo();

protected:
  qreal _oldDx;
  qreal _oldDy;
  qreal _dx;
  qreal _dy;
};

} // namespace SvgAssemblyCommand

} // namespace SvgCompositor

#endif // ASSEMBLY_COMMAND_HPP
