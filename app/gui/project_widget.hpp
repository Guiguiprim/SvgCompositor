#ifndef SVGCOMPOSITOR_PROJECT_WIDGET_HPP
#define SVGCOMPOSITOR_PROJECT_WIDGET_HPP

#include <QWidget>

class QLineEdit;
class QModelIndex;
class QStandardItemModel;
class QTreeView;
class QToolBar;

namespace SvgCompositor
{
class ProjectSortFilterModel;

class ProjectWidget : public QWidget
{
  Q_OBJECT
public:
  explicit ProjectWidget(QWidget *parent = NULL);

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

  void onClicked(const QModelIndex& proxyIndex);
  void onDoubleClicked(const QModelIndex& proxyIndex);

  void setItemModel(QStandardItemModel* model);
  void resort();

Q_SIGNALS:
  void outputDirChanged(const QString& outputDir);
  void customMenuRequested(const QModelIndex& index, const QPoint& pos);

  void addAssembly();
  void removeAssembly();
  void generateAssemblyImage();
  void generateProjectImages();

  void clicked(const QModelIndex& index);
  void doubleClicked(const QModelIndex& index);

private:
  QTreeView* _treeView;
  QToolBar* _toolBar;
  QLineEdit* _outputDir;
  ProjectSortFilterModel* _proxyModel;

  QAction* _addAssemblyAction;
  QAction* _removeAssemblyAction;
  QAction* _generateAssemblyAction;
  QAction* _generateProjectAction;
};

} // namespace SvgCompositor

#endif // SVGCOMPOSITOR_PROJECT_WIDGET_HPP
