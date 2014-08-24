#include "compositor_controller.hpp"

#include <QMetaType>

#include <svg_compose/svg_assembly.hpp>
#include <svg_compose/svg_assemblies_list.hpp>
#include <svg_compose/svg_compose.hpp>

#include "gui/editor.hpp"

namespace SvgCompositor
{
const QString CompositorController::k_windowTitle = "SvgCompositor";

CompositorController::CompositorController(QObject *parent)
  : QObject(parent)
  , _project(NULL)
  , _editors()
{
  qRegisterMetaType<SvgCompose::SvgAssembly*>("SvgCompose::SvgAssembly*");
  qRegisterMetaType<SvgCompose::SvgAssembliesList*>("SvgCompose::SvgAssembliesList*");
}

SvgCompose::SvgAssembliesList* CompositorController::project() const
{
  return _project;
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
  return true;
}

bool CompositorController::openProject(const QString& filename)
{
  if(_project && !closeProject())
    return false;

  QString file = filename;
  if(filename.isEmpty())
  {
    // ask for a project
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

  QString filename;
  // ask for a new file project

  _project->save(filename);
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

bool CompositorController::closeProject()
{
  // Check for save all or cancel !
  while(!_editors.isEmpty())
    xCloseAssembly(_editors.begin(), true);

  Q_EMIT projectChanged(NULL);
  delete _project;
  _project = NULL;

  updateWindowTitle();
  return true;
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

  Q_EMIT addEditor(editor, assembly->name());

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

  return xCloseAssembly(it);
}

void CompositorController::setOutputDir(const QString& outputDir)
{
  if(_project)
    _project->setOutputDir(outputDir);
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
    QMap<SvgCompose::SvgAssembly*, Editor*>::iterator it,
    bool force)
{
  // Check for save or cancel !
  Q_UNUSED(force)

  disconnect(it.key(), SIGNAL(nameChanged(QString)),
             this, SLOT(xOnAssemblyChanged()));
  disconnect(it.key(), SIGNAL(assemblyChanged(bool)),
             this, SLOT(xOnAssemblyChanged()));
  Q_EMIT removeEditor(it.value());
  delete it.value();
  _editors.remove(it.key());
  return true;
}

} // namespace SvgCompositor
