#ifndef SVGCOMPOSITOR_PROJECT_WIDGET_HPP
#define SVGCOMPOSITOR_PROJECT_WIDGET_HPP

#include <QWidget>

class QGroupBox;
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

public Q_SLOTS:
  void setOutputDir(const QString& outputDir);
  void outputDirChanged();
  void onCustomMenuRequested(const QPoint& pos);

Q_SIGNALS:
  void outputDirChanged(const QString& outputDir);
  void customMenuRequested(const QModelIndex& index, const QPoint& pos);

private:
  QTreeView* _treeView;
  QToolBar* _toolBar;
  QGroupBox* _groupBox;
  QLineEdit* _outputDir;
};

} // namespace SvgCompositor

#endif // SVGCOMPOSITOR_PROJECT_WIDGET_HPP
