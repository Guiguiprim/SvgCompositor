#include <svg_compose/svg_compose.hpp>

#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QDomProcessingInstruction>
#include <QFile>

#include <svg_clean/svg_clean.hpp>
#include <svg_compose/svg_assemblies_list.hpp>
#include <svg_compose/svg_assembly.hpp>

#include <QDebug>

namespace SvgCompose
{

void generateAssemblies(
    const SvgAssembliesList* list)
{
  QDir pDir = list->dir();
  QDir oDir = list->outputDir();
  pDir.mkpath(oDir.path());

  Q_FOREACH(SvgAssembly* assembly, list->assemblies())
  {
    generateAssembly(assembly, pDir, oDir);
  }
}

bool generateAssembly(const SvgAssembly* assembly,
    const QDir& projectDir,
    const QDir& outputDir)
{
  QDomDocument svgXml("svg");

  QDomProcessingInstruction declaration = svgXml.createProcessingInstruction(
        "xml", "version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"");

  int size = (int)assembly->size();

  QDomElement root = svgXml.createElement("svg");
  root.setAttribute("version", "1.1");
  root.setAttribute("xmlns", "http://www.w3.org/2000/svg");
  //root.setAttribute("xmlns:svg", "http://www.w3.org/2000/svg"); // normaly usless
  root.setAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
  root.setAttribute("width", QString("%1px").arg(size));
  root.setAttribute("height", QString("%1px").arg(size));

  if(!assembly->background().isEmpty())
    addBackgroundElement(root, projectDir,
                         assembly->background(), size);

  Q_FOREACH(const SvgAssemblyElement& element, assembly->elements())
  {
    addElement(root, projectDir, element);
  }

  svgXml.appendChild(declaration);
  svgXml.appendChild(root);

  QFile svgFile(outputDir.absoluteFilePath(assembly->name()));
  if(!svgFile.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    qDebug() << "Could not open " << svgFile.fileName() << " for writing";
    return false;
  }

  svgFile.write(svgXml.toString().toUtf8().constData());
  svgFile.close();
  return true;
}

bool addBackgroundElement(
    QDomElement& svgRoot,
    const QDir& projectDir,
    const QString& file,
    int size)
{
  QFile backFile(projectDir.absoluteFilePath(file));
  QDomDocument backXml;

  if(!backXml.setContent(&backFile))
  {
    qDebug() << "Could not read background file: " << file;
    return false;
  }
  backFile.close();

  QDomElement rootBack = backXml.documentElement();
  int bh = rootBack.attribute("height", "48px")
           .remove("px", Qt::CaseInsensitive).toInt();
  int bw = rootBack.attribute("width", "48px")
           .remove("px", Qt::CaseInsensitive).toInt();

  QDomElement back = rootBack.cloneNode().toElement();
  SvgClean::cleanElementChildren(back);
  SvgClean::RemoveConfig rc;
  rc.keep_attr.clear();
  rc.addDropedAttribute("xmlns");
  SvgClean::cleanElementAttributes(back, rc);

  qreal sx = (qreal)size / bw;
  qreal sy = (qreal)size / bh;
  qreal s = sx < sy ? sx : sy;

  qreal dx = (size - bw * s) / 2.0;
  qreal dy = (size - bh * s) / 2.0;

  QString transform = QString("translate(%1,%2) scale(%3)")
                      .arg(dx).arg(dy).arg(s);

  QDomElement calque = svgRoot.ownerDocument().createElement("g");
  calque.setAttribute("transform", transform);
  calque.appendChild(back);

  svgRoot.appendChild(calque);

  return true;
}

bool addElement(
    QDomElement& svgRoot,
    const QDir& projectDir,
    const SvgAssemblyElement& element)
{
  QFile elemFile(projectDir.absoluteFilePath(element.file));
  QDomDocument elemXml;

  if(!elemXml.setContent(&elemFile))
  {
    qDebug() << "Could not read background file: " << element.file;
    return false;
  }
  elemFile.close();

  QDomElement rootElem = elemXml.documentElement();

  QDomElement elem = rootElem.cloneNode().toElement();
  SvgClean::cleanElementChildren(elem);
  SvgClean::RemoveConfig rc;
  rc.keep_attr.clear();
  rc.addDropedAttribute("xmlns");
  SvgClean::cleanElementAttributes(elem, rc);

  QString transform = QString("translate(%1,%2) scale(%3)")
                      .arg(element.dx).arg(element.dy).arg(element.scale);

  QDomElement calque = svgRoot.ownerDocument().createElement("g");
  calque.setAttribute("transform", transform);
  calque.appendChild(elem);

  svgRoot.appendChild(calque);

  return true;
}

} // namespace AR
