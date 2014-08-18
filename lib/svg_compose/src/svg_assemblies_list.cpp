#include <svg_compose/svg_assemblies_list.hpp>

#include <svg_compose/svg_assembly.hpp>

namespace SvgCompose
{

SvgAssembliesList::SvgAssembliesList(QObject *parent)
  : QObject(parent)
  , _fileInfo("")
  , _outputDir()
  , _assemblies()
{
}

SvgAssembliesList::SvgAssembliesList(const QString& filename, QObject *parent)
  : QObject(parent)
  , _fileInfo(filename)
  , _outputDir()
  , _assemblies()
{
}

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
}

void SvgAssembliesList::createNew()
{
  SvgAssembly* assembly = new SvgAssembly(this);
  _assemblies.push_back(assembly);
  emit assemblyCreated(assembly);
}

} // namespace Composition
