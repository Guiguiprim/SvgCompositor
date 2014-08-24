#include <svg_compose/svg_assembly_element.hpp>

#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

namespace SvgCompose
{

SvgAssemblyElement::SvgAssemblyElement()
  : file("")
  , scale(1.0)
  , dx(0.0)
  , dy(0.0)
  , validSize(false)
  , sx(0.0)
  , sy(0.0)
{}

SvgAssemblyElement::SvgAssemblyElement(
    const QString& file)
  : file(file)
  , scale(1.0)
  , dx(0.0)
  , dy(0.0)
  , validSize(false)
  , sx(0.0)
  , sy(0.0)
{}

SvgAssemblyElement::SvgAssemblyElement(
    const SvgAssemblyElement& u)
  : file(u.file)
  , scale(u.scale)
  , dx(u.dx)
  , dy(u.dy)
  , validSize(u.validSize)
  , sx(u.sx)
  , sy(u.sy)
{}

SvgAssemblyElement& SvgAssemblyElement::operator=(
    const SvgAssemblyElement& u)
{
  file = u.file;
  scale = u.scale;
  dx = u.dx;
  dy = u.dy;
  validSize = u.validSize;
  sx = u.sx;
  sy = u.sy;
  return *this;
}

void SvgAssemblyElement::checkSize(const QDir& projectDir)
{
  validSize = false;
  sx = 0.0;
  sy = 0.0;

  QFileInfo fileInfo(projectDir, file);
  if(fileInfo.exists())
  {
    QFile qFile(fileInfo.absoluteFilePath());
    QDomDocument xml;
    if(!xml.setContent(&qFile))
    {
      QTextStream cerr(stderr);
      cerr << "Could not read element file: " << file << endl;
      return;
    }
    qFile.close();

    QDomElement root = xml.documentElement();
    sy = root.attribute("height", "0px").remove("px", Qt::CaseInsensitive).toDouble();
    sx = root.attribute("width", "0px").remove("px", Qt::CaseInsensitive).toDouble();

    if(sx != 0 && sy != 0)
      validSize = true;
  }
}

bool operator==(const SvgAssemblyElement& u1, const SvgAssemblyElement& u2)
{
  if(u1.file != u2.file)
    return false;
  if(u1.scale != u2.scale)
    return false;
  if(u1.dx != u2.dx)
    return false;
  if(u1.dy != u2.dy)
    return false;
  return true;
}

bool operator!=(const SvgAssemblyElement& u1, const SvgAssemblyElement& u2)
{
  return !(u1 == u2);
}

} // namespace Composition
