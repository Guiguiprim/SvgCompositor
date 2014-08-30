#ifndef COMPOSITOR_WIDGET_HPP
#define COMPOSITOR_WIDGET_HPP

#include <QMainWindow>
#include <QMap>

class QAction;
class QStackedWidget;
class QStandardItemModel;
class QTabWidget;

namespace SvgCompose
{
class SvgAssembliesList;
}

namespace SvgCompositor
{
class CompositorController;
class Editor;
class ProjectWidget;
class TreeViewController;

class CompositorWidget : public QMainWindow
{
    Q_OBJECT

public:
  explicit CompositorWidget(QWidget *parent = NULL);

public Q_SLOTS:
  void setWindowTitle(const QString& title);
  void onAddEditor(Editor*, const QString& name);
  void onRenameEditor(Editor*, const QString& name);
  void onRemoveEditor(Editor*);
  void onSetCurrentEditor(Editor*);
  void onProjectChanged(SvgCompose::SvgAssembliesList* _project);

private Q_SLOTS:
  void xSaveCurrentAssembly();
  void xCloseCurrentAssembly();
  void xOnTabCloseRequested(int index);
  void xOnTabChanged(int index);

protected:
  virtual void closeEvent(QCloseEvent* event );

private:
  QStackedWidget* _widgetStack;
  QTabWidget* _tabWidget;
  ProjectWidget* _projectWidget;
  CompositorController* _compositorController;
  TreeViewController* _treeViewController;

  QAction* _newProjectAction;
  QAction* _openProjectAction;
  QAction* _saveAssemblyAction;
  QAction* _saveAllAction;
  QAction* _saveAsAction;
  QAction* _closeAssemblyAction;
  QAction* _closeProjectAction;
  QAction* _quitAction;

  int _emptyIndex;
  int _tabIndex;
};

} // namespace SvgCompositor

#endif // COMPOSITOR_WIDGET_HPP
