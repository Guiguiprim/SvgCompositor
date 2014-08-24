#include "project_widget.hpp"

#include <QGroupBox>
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
  , _groupBox(new QGroupBox("Project actions:"))
  , _outputDir(new QLineEdit)
{
  QVBoxLayout* lyt = new QVBoxLayout(this);
  lyt->addWidget(new QLabel("Output dir:"));
  lyt->addWidget(_outputDir);
  lyt->addWidget(_treeView);
  lyt->addWidget(_toolBar);
  lyt->addWidget(_groupBox);

  _treeView->setHeaderHidden(true);
  _treeView->setContextMenuPolicy(Qt::CustomContextMenu);


  QVBoxLayout* gLyt = new QVBoxLayout(_groupBox);
  gLyt->addWidget(new QPushButton("Create SVG"));
  gLyt->addWidget(new QPushButton("Create Icon"));

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

} // namespace SvgCompositor
