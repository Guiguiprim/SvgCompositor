#include "project_widget.hpp"

#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeView>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

#include "icon.hpp"

namespace SvgCompositor
{

ProjectWidget::ProjectWidget(QWidget *parent)
  : QWidget(parent)
  , _treeView(new QTreeView)
  , _toolBar(new QToolBar)
  , _outputDir(new QLineEdit)
{
  QVBoxLayout* lyt = new QVBoxLayout(this);
  lyt->addWidget(new QLabel("Output dir:"));
  lyt->addWidget(_outputDir);
  lyt->addWidget(_treeView);
  lyt->addWidget(_toolBar);

  _treeView->setHeaderHidden(true);
  _treeView->setContextMenuPolicy(Qt::CustomContextMenu);

  _addAssemblyAction = _toolBar->addAction(Icon::add(), "Add new assembly",
                      this, SIGNAL(addAssembly()));
  _addAssemblyAction->setShortcut(QKeySequence::New);
  _removeAssemblyAction = _toolBar->addAction(Icon::remove(), "Remove assembly",
                                      this, SIGNAL(removeAssembly()));
  _removeAssemblyAction->setShortcut(QKeySequence("Ctrl+D"));
  _toolBar->addSeparator();
  _generateAssemblyAction = _toolBar->addAction(Icon::image(), "Generate assembly image",
                                        this, SIGNAL(generateAssemblyImage()));
  _generateProjectAction = _toolBar->addAction(Icon::images(), "Generate project images",
                      this, SIGNAL(generateProjectImages()));
  enableAssemblyAction(false);

  connect(_outputDir, SIGNAL(editingFinished()),
          this, SLOT(outputDirChanged()));
  connect(_treeView, SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(onCustomMenuRequested(QPoint)));
}

QTreeView* ProjectWidget::treeView() const
{
  return _treeView;
}

QString ProjectWidget::outputDir() const
{
  return _outputDir->text();
}

QAction* ProjectWidget::addAssemblyAction() const
{
  return _addAssemblyAction;
}

QAction* ProjectWidget::removeAssemblyAction() const
{
  return _removeAssemblyAction;
}

QAction* ProjectWidget::generateAssemblyAction() const
{
  return _generateAssemblyAction;
}

QAction* ProjectWidget::generateProjectAction() const
{
  return _generateProjectAction;
}

void ProjectWidget::setOutputDir(const QString& outputDir)
{
  _outputDir->setText(outputDir);
}

void ProjectWidget::outputDirChanged()
{
  Q_EMIT outputDirChanged(_outputDir->text());
}

void ProjectWidget::onCustomMenuRequested(const QPoint& pos)
{
  QModelIndex index = _treeView->indexAt(pos);
  QPoint gPos = _treeView->mapToGlobal(pos);
  Q_EMIT customMenuRequested(index, gPos);
}

void ProjectWidget::enableAssemblyAction(bool enable)
{
  _removeAssemblyAction->setEnabled(enable);
  _generateAssemblyAction->setEnabled(enable);
}

} // namespace SvgCompositor
