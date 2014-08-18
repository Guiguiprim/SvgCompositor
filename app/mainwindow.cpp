#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QGraphicsRectItem>
#include <QGraphicsSvgItem>
#include <QStyle>

#include <svg_compose/svg_assembly.hpp>
#include <svg_compose/svg_assemblies_list.hpp>
#include <svg_compose/svg_compose.hpp>

#include "gui/editor.hpp"

#include "icon.hpp"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  ,_ui(new Ui::MainWindow)
  , _composeEditor(new SvgCompositor::Editor)
  , _project(NULL)
{
  _ui->setupUi(this);

  _ui->toolBar->addAction(Icon::buildIcon("plus-green"),"");
  _ui->toolBar->addAction(Icon::buildIcon("minus-red"), "");

  _ui->centralLyt->addWidget(_composeEditor);

  _project = new SvgCompose::SvgAssembliesList("img/project.xml", this);
  _project->createNew();
  _project->setOutputDir("result");
  SvgCompose::SvgAssembly* assembly = _project->assembly(0);

  _composeEditor->setAssembly(assembly);

  assembly->setBackground(QFileInfo("img/background/blue.svg").absoluteFilePath());
  assembly->addElement(QFileInfo("img/logo/add.svg").absoluteFilePath());
  assembly->addElement(QFileInfo("img/logo/connect2.svg").absoluteFilePath());

  //SvgCompose::generateAssemblies(_project);
}

MainWindow::~MainWindow()
{
  delete _ui;
}
