#include "compositor_widget.hpp"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>
#include <QStackedWidget>
#include <QStandardItemModel>
#include <QTabWidget>
#include <QTreeView>
#include <QUndoGroup>

#include <svg_compose/svg_assembly.hpp>
#include <svg_compose/svg_assemblies_list.hpp>
#include <svg_compose/svg_compose.hpp>

#include "controller/compositor_controller.hpp"
#include "controller/tree_view_controller.hpp"
#include "editor.hpp"
#include "project_widget.hpp"
#include "icon.hpp"

namespace SvgCompositor
{

CompositorWidget::CompositorWidget(QWidget *parent)
  : QMainWindow(parent)
  , _widgetStack(new QStackedWidget)
  , _tabWidget(new QTabWidget)
  , _projectWidget(new ProjectWidget)
  , _compositorController(new CompositorController(this))
  , _treeViewController(new TreeViewController(this))
{
  this->QMainWindow::setWindowTitle(CompositorController::k_windowTitle);

  QMenuBar* menuBar = new QMenuBar;
  this->setMenuBar(menuBar);

  QMenu* fileMenu = menuBar->addMenu("File");

  _newProjectAction = fileMenu->addAction(Icon::create(), "New project", _compositorController,
                                          SLOT(createProject()), QKeySequence("Ctrl+Shift+N"));
  _openProjectAction = fileMenu->addAction(Icon::open(), "Open project", _compositorController,
                                           SLOT(openProject()), QKeySequence::Open);
  _saveAssemblyAction = fileMenu->addAction(Icon::save(), "Save assembly in project", this,
                                            SLOT(xSaveCurrentAssembly()), QKeySequence::Save);
  _saveAllAction = fileMenu->addAction(Icon::saveAll(), "Save all assemblies", _compositorController,
                                       SLOT(saveAll()), QKeySequence::SaveAs);
  _saveAsAction = fileMenu->addAction("Save project as", _compositorController,
                                      SLOT(saveProjectAs()));
  _closeAssemblyAction = fileMenu->addAction( "Close current assembly", this,
                                             SLOT(xCloseCurrentAssembly()), QKeySequence::Close);
  _closeProjectAction = fileMenu->addAction(Icon::close(), "Close project", _compositorController,
                                            SLOT(closeProject()), QKeySequence("Ctrl+Shift+W"));
  fileMenu->addSeparator();
  _quitAction = fileMenu->addAction(Icon::exit(), "Quit", this, SLOT(close()), QKeySequence::Quit);

  QMenu* editionMenu = menuBar->addMenu("Edition");

  editionMenu->addAction(_projectWidget->addAssemblyAction());
  editionMenu->addAction(_projectWidget->removeAssemblyAction());
  editionMenu->addAction(_projectWidget->generateAssemblyAction());
  editionMenu->addAction(_projectWidget->generateProjectAction());
  editionMenu->addSeparator();

  QAction* redoAction = _compositorController->undoGroup()->createRedoAction(this);
  redoAction->setIcon(Icon::redo());
  redoAction->setShortcut(QKeySequence::Redo);
  redoAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  editionMenu->addAction(redoAction);

  QAction* undoAction = _compositorController->undoGroup()->createUndoAction(this);
  undoAction->setIcon(Icon::undo());
  undoAction->setShortcut(QKeySequence::Undo);
  undoAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  editionMenu->addAction(undoAction);

  QSplitter* splitter = new QSplitter;
  splitter->addWidget(_projectWidget);
  splitter->setCollapsible(0,false);
  splitter->addWidget(_widgetStack);
  splitter->setCollapsible(1,false);
  splitter->setSizes(QList<int>() << 200 << 600);
  this->setCentralWidget(splitter);


  _emptyIndex = _widgetStack->addWidget(new QWidget);
  _tabIndex = _widgetStack->addWidget(_tabWidget);
  _widgetStack->setCurrentIndex(_emptyIndex);

  _tabWidget->setTabsClosable(true);

  // TabWiget connections
  connect(_tabWidget, SIGNAL(tabCloseRequested(int)),
          this, SLOT(xOnTabCloseRequested(int)));
  connect(_tabWidget, SIGNAL(currentChanged(int)),
          this, SLOT(xOnTabChanged(int)));

  // Compositor Controller connections
  connect(_compositorController, SIGNAL(setWindowTitle(QString)),
          this, SLOT(setWindowTitle(QString)));
  connect(_compositorController, SIGNAL(addEditor(Editor*,QString)),
          this, SLOT(onAddEditor(Editor*,QString)));
  connect(_compositorController, SIGNAL(renameEditor(Editor*,QString)),
          this, SLOT(onRenameEditor(Editor*,QString)));
  connect(_compositorController, SIGNAL(removeEditor(Editor*)),
          this, SLOT(onRemoveEditor(Editor*)));
  connect(_compositorController, SIGNAL(setCurrentEditor(Editor*)),
          this, SLOT(onSetCurrentEditor(Editor*)));

  connect(_compositorController, SIGNAL(outputDirChanged(QString)),
          _projectWidget, SLOT(setOutputDir(QString)));

  connect(_compositorController, SIGNAL(projectChanged(SvgCompose::SvgAssembliesList*)),
          _treeViewController, SLOT(onProjectChanged(SvgCompose::SvgAssembliesList*)));
  connect(_compositorController, SIGNAL(assemblyOpenStatusChanged(SvgCompose::SvgAssembly*,bool)),
          _treeViewController, SLOT(onAssemblyOpenStatusChanged(SvgCompose::SvgAssembly*,bool)));

  connect(_compositorController, SIGNAL(projectChanged(SvgCompose::SvgAssembliesList*)),
          this, SLOT(onProjectChanged(SvgCompose::SvgAssembliesList*)));

  // TreeView Controller connections
  connect(_treeViewController, SIGNAL(openAssembly(SvgCompose::SvgAssembly*)),
          _compositorController, SLOT(openAssembly(SvgCompose::SvgAssembly*)));
  connect(_treeViewController, SIGNAL(showAssembly(SvgCompose::SvgAssembly*)),
          _compositorController, SLOT(showAssembly(SvgCompose::SvgAssembly*)));
  connect(_treeViewController, SIGNAL(generateImage(SvgCompose::SvgAssembly*)),
          _compositorController, SLOT(generateAssemblyImage(SvgCompose::SvgAssembly*)));
  connect(_treeViewController, SIGNAL(removeAssembly(SvgCompose::SvgAssembly*)),
          _compositorController, SLOT(removeAssembly(SvgCompose::SvgAssembly*)));
  connect(_treeViewController, SIGNAL(assemblyAction(SvgCompose::SvgAssembly*,Action,int)),
          _compositorController, SLOT(onAssemblyAction(SvgCompose::SvgAssembly*,Action,int)));
  connect(_treeViewController, SIGNAL(selectElement(SvgCompose::SvgAssembly*,int)),
          _compositorController, SLOT(onSelectElement(SvgCompose::SvgAssembly*,int)));

  connect(_treeViewController, SIGNAL(enableAssemblyActions(bool)),
          _projectWidget, SLOT(enableAssemblyAction(bool)));
  connect(_treeViewController, SIGNAL(modelChanged(QStandardItemModel*)),
          _projectWidget, SLOT(setItemModel(QStandardItemModel*)));

  // projectWidget connections
  connect(_projectWidget, SIGNAL(doubleClicked(QModelIndex)),
          _treeViewController, SLOT(onDoubleClicked(QModelIndex)));
  connect(_projectWidget, SIGNAL(clicked(QModelIndex)),
          _treeViewController, SLOT(onClicked(QModelIndex)));
  connect(_projectWidget, SIGNAL(customMenuRequested(QModelIndex,QPoint)),
          _treeViewController, SLOT(customMenuRequested(QModelIndex,QPoint)));
  connect(_projectWidget, SIGNAL(generateAssemblyImage()),
          _treeViewController, SLOT(onGenerateImageTriggered()));
  connect(_projectWidget, SIGNAL(removeAssembly()),
          _treeViewController, SLOT(onDeleteTriggered()));

  connect(_projectWidget, SIGNAL(generateProjectImages()),
          _compositorController, SLOT(generateProjectImages()));
  connect(_projectWidget, SIGNAL(addAssembly()),
          _compositorController, SLOT(createAssembly()));
  connect(_projectWidget, SIGNAL(outputDirChanged(QString)),
          _compositorController, SLOT(setOutputDir(QString)));

  this->resize(800, 700);
  onProjectChanged(NULL);
}

bool CompositorWidget::openProject(const QString& filename)
{
  return _compositorController->openProject(filename);
}

void CompositorWidget::setWindowTitle(const QString& title)
{
  this->QMainWindow::setWindowTitle(title);
}

void CompositorWidget::onAddEditor(Editor* editor, const QString& name)
{
  _tabWidget->addTab(editor, name);
  _tabWidget->setCurrentWidget(editor);
  if(_widgetStack->currentIndex() == _emptyIndex)
  {
    _widgetStack->setCurrentIndex(_tabIndex);
    _closeAssemblyAction->setEnabled(true);
  }
}

void CompositorWidget::onRenameEditor(Editor* editor, const QString& name)
{
  _tabWidget->setTabText(_tabWidget->indexOf(editor), name);
}

void CompositorWidget::onRemoveEditor(Editor* editor)
{
  _tabWidget->removeTab(_tabWidget->indexOf(editor));
  if(_tabWidget->count() == 0)
  {
    _widgetStack->setCurrentIndex(_emptyIndex);
    _closeAssemblyAction->setEnabled(false);
  }
}

void CompositorWidget::onSetCurrentEditor(Editor* editor)
{
  _tabWidget->setCurrentWidget(editor);
}

void CompositorWidget::onProjectChanged(SvgCompose::SvgAssembliesList* _project)
{
  bool active = _project == NULL ? false : true;
  _projectWidget->setEnabled(active);
  _saveAssemblyAction->setEnabled(active);
  _saveAllAction->setEnabled(active);
  _saveAsAction->setEnabled(active);
  _closeProjectAction->setEnabled(active);
  _projectWidget->addAssemblyAction()->setEnabled(active);
  _projectWidget->generateProjectAction()->setEnabled(active);
  _closeAssemblyAction->setEnabled(false);
}

void CompositorWidget::xSaveCurrentAssembly()
{
  Editor* editor = qobject_cast<Editor*>(_tabWidget->currentWidget());
  if(editor)
    _compositorController->saveAssembly(editor->assembly());
  else
    _compositorController->saveAll();
}

void CompositorWidget::xCloseCurrentAssembly()
{
  Editor* editor = qobject_cast<Editor*>(_tabWidget->currentWidget());
  if(editor)
    _compositorController->closeAssembly(editor->assembly());
}

void CompositorWidget::xOnTabCloseRequested(int index)
{
  Editor* editor = qobject_cast<Editor*>(_tabWidget->widget(index));
  if(editor)
    _compositorController->closeAssembly(editor->assembly());
}

void CompositorWidget::xOnTabChanged(int index)
{
  Editor* editor = qobject_cast<Editor*>(_tabWidget->widget(index));
  if(editor)
    _compositorController->undoGroup()->setActiveStack(editor->undoStack());
}

void CompositorWidget::closeEvent(QCloseEvent* event )
{
  if(_compositorController->project() &&
     _compositorController->project()->hasChanged() &&
     !_compositorController->closeProject())
  {
    event->ignore();
  }
  else
  {
    event->accept();
  }
}

} // namespace SvgCompositor
