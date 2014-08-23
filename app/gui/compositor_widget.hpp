#ifndef COMPOSITOR_WIDGET_HPP
#define COMPOSITOR_WIDGET_HPP

#include <QMainWindow>
#include <QMap>

class QStackedWidget;
class QTabWidget;

namespace Ui
{
class MainWindow;
} // namespace Ui

namespace SvgCompositor
{
class CompositorController;
class Editor;

class CompositorWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit CompositorWidget(QWidget *parent = NULL);
    ~CompositorWidget();

public Q_SLOTS:
  void onAddEditor(Editor*, const QString& name);
  void onRenameEditor(Editor*, const QString& name);
  void onRemoveEditor(Editor*);
  void onSetCurrentEditor(Editor*);

private Q_SLOTS:
  void xOnTabCloseRequested(int index);

private:
    Ui::MainWindow *_ui;
    QStackedWidget* _widgetStack;
    QTabWidget* _tabWidget;
    CompositorController* _controller;

    int _emptyIndex;
    int _tabIndex;
};

} // namespace SvgCompositor

#endif // COMPOSITOR_WIDGET_HPP
