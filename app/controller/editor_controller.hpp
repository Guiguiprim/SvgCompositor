#ifndef EDITOR_CONTROLLER_HPP
#define EDITOR_CONTROLLER_HPP

#include <QObject>

#include "misc/move_struct.hpp"
#include "action.hpp"

class QUndoStack;
class QWidget;

namespace SvgCompose
{
class SvgAssembly;
} // namespace SvgCompose

namespace SvgCompositor
{

class EditorController : public QObject
{
  Q_OBJECT
public:
  explicit EditorController(QWidget *parent = 0);

  void setAssembly(SvgCompose::SvgAssembly* assembly);

  QUndoStack* undoStack() const;

public Q_SLOTS:
  void onNameEdited(const QString& name);
  void onAction(int index, Action action);
  void onActionValue(int index, Action action, qreal value);
  void onElementsMoved(ItemsMove itemsMove);

private:
  QWidget* _parentWidget;
  SvgCompose::SvgAssembly* _assembly;
  QUndoStack* _undoStack;
};

} // namespace SvgCompositor

#endif // EDITOR_CONTROLLER_HPP
