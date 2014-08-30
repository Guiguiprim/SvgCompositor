#include "compositor_controller.hpp"

#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include <QUndoGroup>

#include <svg_compose/svg_assembly.hpp>
#include <svg_compose/svg_assemblies_list.hpp>
#include <svg_compose/svg_compose.hpp>

#include "gui/editor.hpp"
#include "misc/last_dir.hpp"

namespace SvgCompositor
{
const QString CompositorController::k_windowTitle = "SvgCompositor";

CompositorController::CompositorController(QWidget *parent)
  : QObject(parent)
  , _parentWidget(parent)
  , _project(NULL)
  , _editors()
  , _undoGroup(new QUndoGroup(this))
{
  qRegisterMetaType<SvgCompose::SvgAssembly*>("SvgCompose::SvgAssembly*");
  qRegisterMetaType<SvgCompose::SvgAssembliesList*>("SvgCompose::SvgAssembliesList*");
}

SvgCompose::SvgAssembliesList* CompositorController::project() const
{
  return _project;
}

QUndoGroup* CompositorController::undoGroup() const
{
  return _undoGroup;
}

void CompositorController::updateWindowTitle()
{
  if(_project)
  {
    QString title = _project->fileName() + " - " + k_windowTitle;
    _project->hasChanged() ? "*" + title : title;
    Q_EMIT setWindowTitle(title);
  }
  else
    Q_EMIT setWindowTitle(k_windowTitle);
}

bool CompositorController::createProject()
{
  if(_project && !closeProject())
    return false;

  QString file = QFileDialog::getSaveFileName(_parentWidget,
                                                  tr("Create project"),
                                                 LastDir::getProjectDir(),
                                                  tr("Project") + " (*.cr)");
  if(file.isEmpty())
    return false;
  LastDir::saveProjectDir(QFileInfo(file).absolutePath());

  if(!file.endsWith(".cr", Qt::CaseInsensitive))
    file += ".cr";

  _project = new SvgCompose::SvgAssembliesList(file, this);
  Q_EMIT projectChanged(_project);
  updateWindowTitle();
  connect(_project, SIGNAL(outputDirChanged(QString)),
          this, SIGNAL(outputDirChanged(QString)));
  return true;
}

bool CompositorController::openProject(const QString& filename)
{
  if(_project && !closeProject())
    return false;

  QString file = filename;
  if(filename.isEmpty())
  {
    file = QFileDialog::getOpenFileName(_parentWidget,
                                        tr("Open project"),
                                        LastDir::getProjectDir(),
                                        tr("Project") + " (*.cr)");
    if(file.isEmpty())
      return false;
    LastDir::saveProjectDir(QFileInfo(file).absolutePath());
  }

  _project = new SvgCompose::SvgAssembliesList(file, this);
  if(!_project->load())
  {
    delete _project;
    _project = NULL;
    return false;
  }
  Q_EMIT projectChanged(_project);
  updateWindowTitle();
  Q_EMIT outputDirChanged(_project->dir().relativeFilePath(_project->outputDir().absolutePath()));
  connect(_project, SIGNAL(outputDirChanged(QString)),
          this, SIGNAL(outputDirChanged(QString)));
  return true;
}

bool CompositorController::saveProjectAs()
{
  if(!_project)
    return false;

  QString file = QFileDialog::getSaveFileName(_parentWidget,
                                                  tr("Open project"),
                                                 LastDir::getProjectDir(),
                                                  tr("Project") + " (*.cr)");
  if(file.isEmpty())
    return false;
  LastDir::saveProjectDir(QFileInfo(file).absolutePath());

  if(!file.endsWith(".cr", Qt::CaseInsensitive))
    file += ".cr";

  _project->save(file);
  updateWindowTitle();
  return true;
}

bool CompositorController::saveAssembly(SvgCompose::SvgAssembly* assembly)
{
  if(!_project)
    return false;
  _project->saveAssembly(assembly);
  updateWindowTitle();
  return true;
}

bool CompositorController::saveAll()
{
  if(!_project)
    return false;
  _project->saveAll();
  updateWindowTitle();
  return true;
}

bool CompositorController::closeProject(bool force)
{
  bool toReset = false;
  bool tosave = false;
  if(!force && _project->hasChanged())
  {
    QMessageBox::StandardButton ret = QMessageBox::question(
               _parentWidget, tr("Save changes"), tr("Some assemblies has unsaved changes. Do you want to save them ?"),
               QMessageBox::SaveAll | QMessageBox::Discard | QMessageBox::Cancel);
    if(ret == QMessageBox::Cancel)
      return false;
    else if(ret == QMessageBox::Discard)
      toReset = true;
    else
      _project->saveAll();
  }

  while(!_editors.isEmpty())
  {
    SvgCompose::SvgAssembly* assembly = _editors.begin().key();
    Editor* editor = _editors.begin().value();
    xCloseAssembly(assembly, editor, true);
    if(toReset) // reset the assembly to the last save state
      assembly->resetLastState();
  }

  Q_EMIT projectChanged(NULL);
  delete _project;
  _project = NULL;

  updateWindowTitle();
  return true;
}

void CompositorController::createAssembly()
{
  if(_project)
  {
    SvgCompose::SvgAssembly* assembly = _project->createNew();
    openAssembly(assembly);
  }
}

bool CompositorController::openAssembly(SvgCompose::SvgAssembly* assembly)
{
  if(showAssembly(assembly))
    return true;

  if(!_project->assemblies().contains(assembly))
    return false;

  Editor* editor = new Editor;
  editor->setAssembly(assembly);
  _editors[assembly] = editor;
  _undoGroup->addStack(editor->undoStack());

  Q_EMIT addEditor(editor, assembly->name());
  Q_EMIT assemblyOpenStatusChanged(assembly, true);

  connect(assembly, SIGNAL(nameChanged(QString)),
          this, SLOT(xOnAssemblyChanged()));
  connect(assembly, SIGNAL(assemblyChanged(bool)),
          this, SLOT(xOnAssemblyChanged()));

  return true;
}

bool CompositorController::showAssembly(SvgCompose::SvgAssembly* assembly)
{
  if(!assembly || !_project)
    return false;

  QMap<SvgCompose::SvgAssembly*, Editor*>::iterator it;
  it = _editors.find(assembly);
  if(it != _editors.end())
  {
    Q_EMIT setCurrentEditor(it.value());
    return true;
  }
  return false;
}

bool CompositorController::closeAssembly(SvgCompose::SvgAssembly* assembly)
{
  QMap<SvgCompose::SvgAssembly*, Editor*>::iterator it;
  it = _editors.find(assembly);
  if(it == _editors.end())
    return false;

  return xCloseAssembly(it.key(), it.value());
}

bool CompositorController::removeAssembly(SvgCompose::SvgAssembly* assembly)
{
  if(!_project || !assembly)
    return false;

  QMessageBox::StandardButton ret = QMessageBox::question(
             _parentWidget, tr("Remove assembly"), tr("Do you really want to remove the assembly ?\nThat action can't be undone."),
             QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);

  if(ret != QMessageBox::Ok)
    return false;

  QMap<SvgCompose::SvgAssembly*, Editor*>::iterator it;
  it = _editors.find(assembly);
  if(it != _editors.end())
    xCloseAssembly(it.key(), it.value(), true);

  return _project->removeAssembly(assembly);
}

void CompositorController::setOutputDir(const QString& outputDir)
{
  if(_project)
    _project->setOutputDir(outputDir);
}

void CompositorController::generateAssemblyImage(SvgCompose::SvgAssembly* assembly) const
{
  if(_project && assembly)
  {
    _project->dir().mkpath(_project->outputDir().path()); // ensure that the output dir exists
    SvgCompose::generateAssembly(assembly, _project->dir(), _project->outputDir());
  }
}

void CompositorController::generateProjectImages() const
{
  if(_project)
    SvgCompose::generateAssemblies(_project);
}

void CompositorController::xOnAssemblyChanged()
{
  SvgCompose::SvgAssembly* assembly = qobject_cast<SvgCompose::SvgAssembly*>(sender());
  if(!assembly)
    return;

  QMap<SvgCompose::SvgAssembly*, Editor*>::iterator it;
  it = _editors.find(assembly);
  if(it != _editors.end())
  {
    QString title = assembly->name();
    if(assembly->hasChanged())
      title = "*" + title;
    Q_EMIT renameEditor(it.value(), title);
  }
}

bool CompositorController::xCloseAssembly(
    SvgCompose::SvgAssembly* assembly,
    Editor* editor,
    bool force)
{
  bool toReset = false;
  if(!force && assembly->hasChanged())
  {
    QMessageBox::StandardButton ret = QMessageBox::question(
               _parentWidget, tr("Save changes"), tr("The current assembly has some unsaved changes. Do you want to save them ?"),
               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if(ret == QMessageBox::Cancel)
      return false;
    else if(ret == QMessageBox::Discard)
      toReset = true;
    else
    {
      assembly->saveCurrentState();
      _project->save();
    }
  }

  disconnect(assembly, SIGNAL(nameChanged(QString)),
             this, SLOT(xOnAssemblyChanged()));
  disconnect(assembly, SIGNAL(assemblyChanged(bool)),
             this, SLOT(xOnAssemblyChanged()));
  Q_EMIT removeEditor(editor);
  _undoGroup->removeStack(editor->undoStack());
  _editors.remove(assembly);
  delete editor;
  Q_EMIT assemblyOpenStatusChanged(assembly, false);

  if(toReset) // reset the assembly to the last save state
    assembly->resetLastState();

  return true;
}

} // namespace SvgCompositor
