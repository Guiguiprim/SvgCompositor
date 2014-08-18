#include <svg_clean/svg_clean.hpp>

#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QFile>
#include <QFileInfo>
#include <QRegExp>
#include <QTextStream>

#include <QDebug>


SvgClean::RemoveConfig::RemoveConfig()
{
  addAllowedNamespace("svg");
  addAllowedNamespace("xlink");
  addDropedTag("metadata");
}

void SvgClean::RemoveConfig::addAllowedNamespace(const QString& name)
{
  keep_attr << name + ":" << "xmlns:" + name;
  keep_tag << name + ":";
}

void SvgClean::RemoveConfig::addDropedAttribute(const QString& name)
{
  drop_attr << name;
}

void SvgClean::RemoveConfig::addDropedTag(const QString& name)
{
  drop_tag << name;
}

bool SvgClean::clean(
    const QString& fileNameIn,
    const QString& fileNameOut,
    const RemoveConfig& rc)
{
  QFile fileIn(fileNameIn);
  QDomDocument xmlIn;

  if(!xmlIn.setContent(&fileIn))
  {
    QTextStream cerr(stderr);
    cerr << "Could not read input file" << endl;
    return false;
  }
  fileIn.close();

  QDomDocument xmlOut = clean(xmlIn, rc);
  QFile fileOut(fileNameOut);
  if(!fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    QTextStream cerr(stderr);
    cerr << "Could not open " << fileOut.fileName() << " for writing" << endl;
    return false;
  }

  fileOut.write(xmlOut.toString().toUtf8().constData());
  fileOut.close();
  return true;
}

bool SvgClean::clean(
    const QString& fileNameIn,
    const RemoveConfig& rc)
{
  QFile fileIn(fileNameIn);
  QDomDocument xmlIn;

  if(!xmlIn.setContent(&fileIn))
  {
    QTextStream cerr(stderr);
    cerr << "Could not read input file" << endl;
    return false;
  }
  fileIn.close();

  QDomDocument xmlOut = clean(xmlIn, rc);
  QTextStream cout(stdout);
  cout << xmlOut.toString().toUtf8().constData();
}

QDomDocument SvgClean::clean(
    const QDomDocument& xmlIn,
    const RemoveConfig& rc)
{
  QDomDocument xmlOut("svg");
  QDomElement rootIn = xmlIn.documentElement().cloneNode().toElement();

  cleanElementAttributes(rootIn, rc);
  cleanElementChildren(rootIn, rc);

  QDomProcessingInstruction declaration = xmlOut.createProcessingInstruction(
        "xml", "version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"");

  xmlOut.appendChild(declaration);
  xmlOut.appendChild(rootIn);
  return xmlOut;
}

void SvgClean::cleanElementChildren(
    QDomElement& parent,
    const RemoveConfig& rc)
{
  QDomNodeList nodes = parent.childNodes();
  for(int i=0; i<nodes.count();)
  {
    QDomNode node = nodes.at(i);
    if(node.isElement())
    {
      QDomElement elem = node.toElement();
      if(isToRemoveTag(elem.tagName(), rc))
        parent.removeChild(elem);
      else
      {
        cleanElementAttributes(elem, rc);
        cleanElementChildren(elem, rc);
         ++i;
      }
    }
    else
      ++i;
  }
}

void SvgClean::cleanElementAttributes(
    QDomElement& elem,
    const RemoveConfig& rc)
{
  QRegExp reg("([0-9]*\\.[0-9]{3})[0-9]{1,}");
  QDomNamedNodeMap attributes = elem.attributes();
  for(int j=0; j<attributes.count();)
  {
    QDomAttr attr = attributes.item(j).toAttr();
    if(isToRemoveAttribute(attr.name(), rc))
      elem.removeAttribute(attr.name());
    else
    {
      QString value = attr.value();
      while(reg.indexIn(value) > -1)
        value.replace(reg.cap(0), reg.cap(1));
      elem.setAttribute(attr.name(), value);
      ++j;
    }
  }
}

bool SvgClean::isToRemoveTag(
    const QString& name,
    const RemoveConfig& rc)
{
  Q_FOREACH(QString d, rc.drop_tag)
  {
    if(name == d)
      return true;
  }

  Q_FOREACH(QString k, rc.keep_tag)
  {
    if(name.contains(k))
      return false;
  }

  if(name.contains(":"))
    return true;

  return false;
}

bool SvgClean::isToRemoveAttribute(
    const QString& name,
    const RemoveConfig& rc)
{
  Q_FOREACH(QString d, rc.drop_attr)
  {
    if(name == d)
      return true;
  }

  Q_FOREACH(QString k, rc.keep_attr)
  {
    if(name.contains(k))
      return false;
  }

  if(name.contains(":"))
    return true;

  return false;
}
