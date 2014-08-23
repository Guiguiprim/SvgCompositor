#include "project_widget.hpp"

#include <QGroupBox>
#include <QPushButton>
#include <QTreeView>
#include <QToolBar>
#include <QVBoxLayout>

#include "icon.hpp"

namespace SvgCompositor
{

ProjectWidget::ProjectWidget(QWidget *parent)
  : QWidget(parent)
  , _treeView(new QTreeView)
  , _toolBar(new QToolBar)
  , _groupBox(new QGroupBox("Project actions:"))
{
  QVBoxLayout* lyt = new QVBoxLayout(this);
  lyt->addWidget(_treeView);
  lyt->addWidget(_toolBar);
  lyt->addWidget(_groupBox);

  _treeView->setHeaderHidden(true);

  QVBoxLayout* gLyt = new QVBoxLayout(_groupBox);
  gLyt->addWidget(new QPushButton("Create SVG"));
  gLyt->addWidget(new QPushButton("Create Icon"));
}

QTreeView* ProjectWidget::treeView() const
{
  return _treeView;
}

} // namespace SvgCompositor
