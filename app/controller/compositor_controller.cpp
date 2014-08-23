#include "compositor_controller.hpp"

#include <QMetaType>

#include <svg_compose/svg_assembly.hpp>
#include <svg_compose/svg_assemblies_list.hpp>
#include <svg_compose/svg_compose.hpp>

#include "gui/editor.hpp"

namespace SvgCompositor
{

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

bool CompositorController::openProject(const QString& filename)
{
  if(_project && !closeProject())
    return false;

  _project = new SvgCompose::SvgAssembliesList(filename, this);
  if(!_project->load())
  {
    delete _project;
    _project = NULL;
    return false;
  }
  Q_EMIT projectChanged(_project);
  return true;
}

bool CompositorController::saveProjectAs(const QString& filename)
{
  if(!_project)
    return false;
  _project->save(filename);
  return true;
}

bool CompositorController::saveAssembly(SvgCompose::SvgAssembly* assembly)
{
  if(!_project)
    return false;
  _project->saveAssembly(assembly);
  return true;
}

bool CompositorController::saveAllProject()
{
  if(!_project)
    return false;
  _project->saveAll();
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

  return true;
}

bool CompositorController::openAssembly(SvgCompose::SvgAssembly* assembly)
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

  if(!_project->assemblies().contains(assembly))
    return false;

  Editor* editor = new Editor;
  editor->setAssembly(assembly);
  _editors[assembly] = editor;

  Q_EMIT addEditor(editor, assembly->name());

  connect(assembly, SIGNAL(nameChanged(QString)),
          this, SLOT(xOnAssemblyNameChanged(QString)));

  return true;
}

bool CompositorController::closeAssembly(SvgCompose::SvgAssembly* assembly)
{
  QMap<SvgCompose::SvgAssembly*, Editor*>::iterator it;
  it = _editors.find(assembly);
  if(it == _editors.end())
    return false;

  return xCloseAssembly(it);
}

void CompositorController::xOnAssemblyNameChanged(const QString& name)
{
  SvgCompose::SvgAssembly* assembly = qobject_cast<SvgCompose::SvgAssembly*>(sender());
  if(!assembly)
    return;

  QMap<SvgCompose::SvgAssembly*, Editor*>::iterator it;
  it = _editors.find(assembly);
  if(it != _editors.end())
  {
    Q_EMIT renameEditor(it.value(), name);
  }
}

bool CompositorController::xCloseAssembly(
    QMap<SvgCompose::SvgAssembly*, Editor*>::iterator it,
    bool force)
{
  // Check for save or cancel !
  Q_UNUSED(force)

  disconnect(it.key(), SIGNAL(nameChanged(QString)),
             this, SLOT(xOnAssemblyNameChanged(QString)));
  Q_EMIT removeEditor(it.value());
  delete it.value();
  _editors.remove(it.key());
  return true;
}

} // namespace SvgCompositor
