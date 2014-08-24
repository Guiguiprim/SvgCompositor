#ifndef SVGCOMPOSITOR_PROJECT_WIDGET_HPP
#define SVGCOMPOSITOR_PROJECT_WIDGET_HPP

#include <QWidget>

class QLineEdit;
class QModelIndex;
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
  QString outputDir() const;

  QAction* addAssemblyAction() const;
  QAction* removeAssemblyAction() const;
  QAction* generateAssemblyAction() const;
  QAction* generateProjectAction() const;

public Q_SLOTS:
  void setOutputDir(const QString& outputDir);
  void outputDirChanged();
  void onCustomMenuRequested(const QPoint& pos);

  void enableAssemblyAction(bool enable);

Q_SIGNALS:
  void outputDirChanged(const QString& outputDir);
  void customMenuRequested(const QModelIndex& index, const QPoint& pos);

  void addAssembly();
  void removeAssembly();
  void generateAssemblyImage();
  void generateProjectImages();

private:
  QTreeView* _treeView;
  QToolBar* _toolBar;
  QLineEdit* _outputDir;

  QAction* _addAssemblyAction;
  QAction* _removeAssemblyAction;
  QAction* _generateAssemblyAction;
  QAction* _generateProjectAction;
};

} // namespace SvgCompositor

#endif // SVGCOMPOSITOR_PROJECT_WIDGET_HPP
