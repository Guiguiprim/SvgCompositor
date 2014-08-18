#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace SvgCompositor
{
class Editor;
}

namespace SvgCompose {
class SvgAssembliesList;
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

protected:

private:
    Ui::MainWindow *_ui;
    SvgCompositor::Editor* _composeEditor;
    SvgCompose::SvgAssembliesList* _project;
};

#endif // MAINWINDOW_HPP
