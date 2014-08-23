#ifndef SVGCOMPOSITOR_PROJECT_WIDGET_HPP
#define SVGCOMPOSITOR_PROJECT_WIDGET_HPP

#include <QWidget>

class QGroupBox;
class QTreeView;
class QToolBar;

namespace SvgCompositor
{

class ProjectWidget : public QWidget
{
  Q_OBJECT
public:
  explicit ProjectWidget(QWidget *parent = NULL);

  QTreeView* treeView() const;

private:
  QTreeView* _treeView;
  QToolBar* _toolBar;
  QGroupBox* _groupBox;
};

} // namespace SvgCompositor

#endif // SVGCOMPOSITOR_PROJECT_WIDGET_HPP
