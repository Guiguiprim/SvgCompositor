#include <svg_compose/svg_assemblies_list.hpp>

#include <QDomDocument>
#include <QDomElement>

#include <svg_compose/svg_assembly.hpp>

#include <QDebug>

namespace SvgCompose
{

SvgAssembliesList::SvgAssembliesList(QObject *parent)
  : QObject(parent)
  , _fileInfo("untitled.cr")
  , _outputDir()
  , _assemblies()
{
}

SvgAssembliesList::SvgAssembliesList(const QString& filename, QObject *parent)
  : QObject(parent)
  , _fileInfo(filename)
  , _outputDir()
  , _assemblies()
{}

QString SvgAssembliesList::baseName() const
{
  return _fileInfo.baseName();
}

QString SvgAssembliesList::fileName() const
{
  return _fileInfo.fileName();
}

QString SvgAssembliesList::filePath() const
{
  return _fileInfo.filePath();
}

QDir SvgAssembliesList::dir() const
{
  return _fileInfo.absoluteDir();
}

QDir SvgAssembliesList::outputDir() const
{
  return QDir(dir().absoluteFilePath(_outputDir));
}

SvgAssembly* SvgAssembliesList::assembly(int index) const
{
  if(index >= 0 && index < _assemblies.count())
    return _assemblies.at(index);
  return NULL;
}

SvgAssembly* SvgAssembliesList::assembly(const QString& name) const
{
  Q_FOREACH(SvgAssembly* assembly, _assemblies)
  {
    if(assembly->name() == name)
      return assembly;
  }
  return NULL;
}

QVector<SvgAssembly*> SvgAssembliesList::assemblies() const
{
  return _assemblies;
}

int SvgAssembliesList::count() const
{
  return _assemblies.count();
}

int SvgAssembliesList::indexOf(SvgAssembly* assembly) const
{
  return _assemblies.indexOf(assembly);
}

void SvgAssembliesList::setFileName(const QString& filename)
{
  _fileInfo = QFileInfo(filename);
}

void SvgAssembliesList::setOutputDir(const QString& path)
{
  _outputDir = dir().relativeFilePath(path);
  Q_EMIT outputDirChanged(_outputDir);
}

bool SvgAssembliesList::load(const QString& filename)
{
  if(!filename.isEmpty())
    setFileName(filename);

  if(!_fileInfo.exists())
  {
    qDebug() << "Project document doesn't exist: " << _fileInfo.fileName();
    return false;
  }

  QFile input(_fileInfo.filePath());
  QDomDocument doc;
  if(!doc.setContent(&input))
  {
    qDebug() << "Could not read project document: " << _fileInfo.fileName();
    return false;
  }
  input.close();

  QDomElement root = doc.documentElement();
  _outputDir = root.attribute("outputDir", "");

  QDomElement elem = root.firstChildElement(SvgAssembly::k_tag);
  while(!elem.isNull())
  {
    SvgAssembly* assembly = new SvgAssembly(this);
    if(assembly->load(elem))
    {
      _assemblies.push_back(assembly);
      emit assemblyAdded(assembly);
    }
    else
    {
      delete assembly;
    }
    elem = elem.nextSiblingElement(SvgAssembly::k_tag);
  }

  return true;
}

bool SvgAssembliesList::save(const QString& filename)
{
  if(!filename.isEmpty())
    setFileName(filename);

  QDomDocument doc;
  QDomElement root = doc.createElement("assemblies");
  root.setAttribute("outputDir", _outputDir);

  QDomProcessingInstruction declaration = doc.createProcessingInstruction(
                                            "xml", "version=\"1.0\" encoding=\"UTF-8\"");
  doc.appendChild(declaration);

  Q_FOREACH(SvgAssembly* assemblie, _assemblies)
  {
    root.appendChild(assemblie->createDom(true));
  }
  doc.appendChild(root);

  QFile output(_fileInfo.filePath());
  if(!output.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    qDebug() << "Could not open " << output.fileName() << " for writing";
    return false;
  }
  output.write(doc.toString().toUtf8().constData());
  output.close();
  return true;
}

bool SvgAssembliesList::hasChanged() const
{
  Q_FOREACH(SvgAssembly* assembly, _assemblies)
  {
    if(assembly->hasChanged())
      return true;
  }

  return false;
}

void SvgAssembliesList::saveAssembly(SvgAssembly* assembly)
{
  if(!assembly || !_assemblies.contains(assembly))
    return;
  assembly->saveCurrentState();
  save();
}

void SvgAssembliesList::saveAll()
{
  Q_FOREACH(SvgAssembly* assembly, _assemblies)
  {
    assembly->saveCurrentState();
  }
  save();
}

SvgAssembly* SvgAssembliesList::createNew()
{
  SvgAssembly* assembly = new SvgAssembly(this);
  _assemblies.push_back(assembly);
  emit assemblyAdded(assembly);
  return assembly;
}

bool SvgAssembliesList::removeAssembly(SvgCompose::SvgAssembly* assembly)
{
  int index = _assemblies.indexOf(assembly);
  if(index == -1)
    return false;

  _assemblies.remove(index);
  Q_EMIT assemblyRemoved(assembly);
  delete assembly;

  return true;
}

} // namespace Composition
