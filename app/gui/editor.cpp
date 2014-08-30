#include "editor.hpp"

#include <QAction>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QUndoStack>
#include <QVBoxLayout>

#include <svg_compose/svg_assembly.hpp>

#include "icon.hpp"
#include "controller/button_bar_controller.hpp"
#include "controller/editor_controller.hpp"
#include "controller/view_controller.hpp"
#include "misc/move_struct.hpp"
#include "button_bar.hpp"
#include "graphics_item.hpp"
#include "graphics_view.hpp"

namespace SvgCompositor
{

Editor::Editor(QWidget *parent)
  :  QWidget(parent)
  , _view(NULL)
  , _buttonBar(NULL)
  , _xmlPreview(NULL)
  , _assembly(NULL)
  , _viewController(NULL)
  , _editorController(NULL)
  , _buttonBarController(NULL)
{
  qRegisterMetaType<ItemMove>("ItemMove");
  qRegisterMetaType<ItemsMove>("ItemsMove");

  // Widgets
  _view = new GraphicsView;

  _buttonBar = new ButtonBar;
  _buttonBar->setEnabled(false);

  _xmlPreview = new QTextEdit;
  _xmlPreview->setReadOnly(true);

  QVBoxLayout* vLyt = new QVBoxLayout(this);
  vLyt->setMargin(0);
  vLyt->setSpacing(1);
  vLyt->addWidget(_view, 4);
  vLyt->addWidget(_buttonBar);
  vLyt->addWidget(_xmlPreview, 1);

  // Controllers
  _viewController = new ViewController(_view, this);

  _buttonBarController = new ButtonBarController(_buttonBar, this);
  connect(_viewController, SIGNAL(selectionChanged(int)),
          _buttonBarController, SLOT(onSelectionChanged(int)));

  _editorController = new EditorController(this);
  connect(_viewController, SIGNAL(elementsMoved(ItemsMove)),
          _editorController, SLOT(onElementsMoved(ItemsMove)));
  connect(_buttonBarController, SIGNAL(actionTriggered(int,Action)),
          _editorController, SLOT(onAction(int,Action)));
  connect(_buttonBarController, SIGNAL(actionValueTriggered(int,Action,qreal)),
          _editorController, SLOT(onActionValue(int,Action,qreal)));
  connect(_buttonBarController, SIGNAL(nameChanged(QString)),
          _editorController, SLOT(onNameEdited(QString)));

  QAction* redoAction = _editorController->undoStack()->createRedoAction(this);
  redoAction->setShortcut(QKeySequence::Redo);
  redoAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  this->addAction(redoAction);

  QAction* undoAction = _editorController->undoStack()->createUndoAction(this);
  undoAction->setShortcut(QKeySequence::Undo);
  undoAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  this->addAction(undoAction);
}

Editor::~Editor()
{}

QUndoStack* Editor::undoStack() const
{
  return _editorController->undoStack();
}

void Editor::setAssembly(SvgCompose::SvgAssembly* assembly)
{
  if(_assembly)
  {
    _viewController->setAssembly(NULL);
    _editorController->setAssembly(NULL);
    _buttonBarController->setAssembly(NULL);

    _xmlPreview->setPlainText("");
    disconnect(_assembly, SIGNAL(assemblyChanged(QString)),
               _xmlPreview, SLOT(setPlainText(QString)));
  }
  _assembly = assembly;
  if(_assembly)
  {
    _viewController->setAssembly(_assembly);
    _editorController->setAssembly(_assembly);
    _buttonBarController->setAssembly(_assembly);

    _xmlPreview->setPlainText(assembly->xml());
    connect(_assembly, SIGNAL(assemblyChanged(QString)),
            _xmlPreview, SLOT(setPlainText(QString)));
  }
}

SvgCompose::SvgAssembly* Editor::assembly() const
{
  return _assembly;
}

void Editor::assemblyAction(int index, Action action)
{
  _editorController->onAction(index, action);
}

void Editor::selectElement(int index)
{
  _viewController->selectElement(index);
}

} // namespace SvgCompositor
