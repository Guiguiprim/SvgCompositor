#include "compositor_widget.hpp"

#include <QApplication>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QStandardItemModel>
#include <QTabWidget>
#include <QTreeView>

#include <svg_compose/svg_assembly.hpp>
#include <svg_compose/svg_assemblies_list.hpp>
#include <svg_compose/svg_compose.hpp>

#include "controller/compositor_controller.hpp"
#include "controller/tree_view_controller.hpp"
#include "editor.hpp"
#include "project_widget.hpp"

namespace SvgCompositor
{

CompositorWidget::CompositorWidget(QWidget *parent)
  : QMainWindow(parent)
  , _widgetStack(new QStackedWidget)
  , _tabWidget(new QTabWidget)
  , _projectWidget(new ProjectWidget)
  , _controller(new CompositorController(this))
  , _treeViewController(new TreeViewController(this))
{

  QWidget* widget = new QWidget;
  this->setCentralWidget(widget);

  QHBoxLayout* lyt = new QHBoxLayout(widget);
  lyt->setMargin(0);
  lyt->addWidget(_projectWidget, 1);
  lyt->addWidget(_widgetStack, 3);

  _emptyIndex = _widgetStack->addWidget(new QWidget);
  _tabIndex = _widgetStack->addWidget(_tabWidget);
  _widgetStack->setCurrentIndex(_emptyIndex);

  _tabWidget->setTabsClosable(true);

  connect(_tabWidget, SIGNAL(tabCloseRequested(int)),
          this, SLOT(xOnTabCloseRequested(int)));

  connect(_controller, SIGNAL(addEditor(Editor*,QString)),
          this, SLOT(onAddEditor(Editor*,QString)));
  connect(_controller, SIGNAL(renameEditor(Editor*,QString)),
          this, SLOT(onRenameEditor(Editor*,QString)));
  connect(_controller, SIGNAL(removeEditor(Editor*)),
          this, SLOT(onRemoveEditor(Editor*)));
  connect(_controller, SIGNAL(setCurrentEditor(Editor*)),
          this, SLOT(onSetCurrentEditor(Editor*)));

  connect(_controller, SIGNAL(projectChanged(SvgCompose::SvgAssembliesList*)),
          _treeViewController, SLOT(onProjectChanged(SvgCompose::SvgAssembliesList*)));

  connect(_treeViewController, SIGNAL(modelChanged(QStandardItemModel*)),
          this, SLOT(xOnModelChanged(QStandardItemModel*)));

  _controller->openProject("img/project.cr");
  SvgCompose::SvgAssembliesList* project = _controller->project();
  if(project)
  {
    Q_FOREACH(SvgCompose::SvgAssembly* assembly, project->assemblies())
    {
      _controller->openAssembly(assembly);
    }
  }

  //SvgCompose::generateAssemblies(project);
}

void CompositorWidget::onAddEditor(Editor* editor, const QString& name)
{
  _tabWidget->addTab(editor, name);
  _tabWidget->setCurrentWidget(editor);
  if(_widgetStack->currentIndex() == _emptyIndex)
    _widgetStack->setCurrentIndex(_tabIndex);
}

void CompositorWidget::onRenameEditor(Editor* editor, const QString& name)
{
  _tabWidget->setTabText(_tabWidget->indexOf(editor), name);
}

void CompositorWidget::onRemoveEditor(Editor* editor)
{
  _tabWidget->removeTab(_tabWidget->indexOf(editor));
  if(_tabWidget->count() == 0)
    _widgetStack->setCurrentIndex(_emptyIndex);
}

void CompositorWidget::onSetCurrentEditor(Editor* editor)
{
  _tabWidget->setCurrentWidget(editor);
}

void CompositorWidget::xOnTabCloseRequested(int index)
{
  Editor* editor = qobject_cast<Editor*>(_tabWidget->widget(index));
  if(editor)
    _controller->closeAssembly(editor->assembly());
}

void CompositorWidget::xOnModelChanged(QStandardItemModel* model)
{
  _projectWidget->treeView()->setModel(model);
}

} // namespace SvgCompositor
