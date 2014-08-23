#include "compositor_widget.hpp"
#include "ui_mainwindow.h"

#include <QStackedWidget>
#include <QTabWidget>

#include <svg_compose/svg_assembly.hpp>
#include <svg_compose/svg_assemblies_list.hpp>
#include <svg_compose/svg_compose.hpp>

#include "controller/compositor_controller.hpp"
#include "gui/editor.hpp"
#include "icon.hpp"

namespace SvgCompositor
{

CompositorWidget::CompositorWidget(QWidget *parent)
  : QMainWindow(parent)
  ,_ui(new Ui::MainWindow)
  , _widgetStack(new QStackedWidget)
  , _tabWidget(new QTabWidget)
  , _controller(new CompositorController(this))
{
  _ui->setupUi(this);

  _ui->toolBar->addAction(Icon::buildIcon("plus-green"),"");
  _ui->toolBar->addAction(Icon::buildIcon("minus-red"), "");

  _ui->centralLyt->addWidget(_widgetStack);

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

  _controller->openProject("img/project.xml");
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

CompositorWidget::~CompositorWidget()
{
  delete _ui;
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

} // namespace SvgCompositor
