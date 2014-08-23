#include <svg_compose/svg_assembly.hpp>

#include <QDir>
#include <QFileInfo>
#include <QTextStream>

#include <svg_compose/svg_assemblies_list.hpp>

namespace SvgCompose
{
QDomDocument SvgAssembly::s_dummyDoc = QDomDocument();
const QString SvgAssembly::k_tag = "assembly";

SvgAssembly::SvgAssembly(SvgAssembliesList* project)
  : QObject(project)
  , _name("Untitled")
  , _size(S48)
  , _background("")
  , _elements()
  , _project(project)
  , _hasChanged(false)
{
}

SvgAssembly::SvgAssembly(SvgAssembliesList* project, const QString& name)
  : QObject(project)
  , _name(name)
  , _size(S48)
  , _background("")
  , _elements()
  , _project(project)
  , _hasChanged(false)
{
}

QString SvgAssembly::name() const
{
  return _name;
}

int SvgAssembly::sizeInt() const
{
  return (int)_size;
}

SvgAssemblySize SvgAssembly::size() const
{
  return _size;
}

QString SvgAssembly::background() const
{
  return _background;
}

SvgAssemblyElement SvgAssembly::element(int index) const
{
  if(xIndexValid(index))
    return _elements.at(index);
  return SvgAssemblyElement();
}

QVector<SvgAssemblyElement> SvgAssembly::elements() const
{
  return _elements;
}

int SvgAssembly::elementsCount() const
{
  return _elements.count();
}

SvgAssembliesList* SvgAssembly::project() const
{
  return _project;
}

void SvgAssembly::setHasChanged(bool changed)
{
  if(_hasChanged != changed)
  {
    _hasChanged = changed;
    Q_EMIT assemblyChanged(_hasChanged);
  }
}

bool SvgAssembly::hasChanged() const
{
  return _hasChanged;
}

bool SvgAssembly::load(QDomElement& root)
{
  if(root.tagName() != k_tag)
    return false;

  QString name = root.attribute("name");
  if(name.isEmpty())
    return false;

  int size = root.attribute("size", "48").toInt();

  QString background;
  QDomElement backElem = root.firstChildElement("background");
  if(!backElem.isNull())
  {
    background = backElem.attribute("path", "");
  }

  SvgAssemblyElement u;
  QVector<SvgAssemblyElement> unions;
  QDomElement elem = root.firstChildElement("element");
  while(!elem.isNull())
  {
    u.file = elem.attribute("path", "");
    if(!u.file.isEmpty())
    {
      u.scale = elem.attribute("scale", "1").toDouble();
      u.dx = elem.attribute("dx", "0").toDouble();
      u.dy = elem.attribute("dy", "0").toDouble();
      u.checkSize(_project->dir());
      unions.push_back(u);
    }
    elem = elem.nextSiblingElement("element");
  }

  _name = name;
  _size = toComposeSize(size);
  _background = background;
  _elements = unions;

  _hasChanged = false;
  emit assemblyLoaded();
  return true;
}

QDomDocumentFragment SvgAssembly::createDom() const
{
  QDomDocumentFragment docFrag = emptyFrag();
  QDomElement root = s_dummyDoc.createElement(k_tag);
  root.setAttribute("name", _name);
  root.setAttribute("size", (int)_size);

  if(!_background.isEmpty())
  {
      QDomElement backElem = s_dummyDoc.createElement("background");
      backElem.setAttribute("path", _background);
      root.appendChild(backElem);
  }

  Q_FOREACH(SvgAssemblyElement uElem, _elements)
  {
      QDomElement dElem = s_dummyDoc.createElement("element");
      dElem.setAttribute("path", uElem.file);
      if(uElem.dx != 0)
          dElem.setAttribute("dx", QString::number(uElem.dx, 'f', 1));
      if(uElem.dy != 0)
          dElem.setAttribute("dy", QString::number(uElem.dy, 'f', 1));
      if(uElem.scale != 1)
          dElem.setAttribute("scale", QString::number(uElem.scale, 'f', 3));
      root.appendChild(dElem);
  }

  docFrag.appendChild(root);
  return docFrag;
}

QString SvgAssembly::xml() const
{
  QString str;
  QTextStream stream(&str);
  QDomDocumentFragment elem = createDom();
  elem.save(stream, 2);
  return str;
}

void SvgAssembly::setName(const QString& name)
{
  if(!name.isEmpty() && _name != name)
  {
    _name = name;
    emit nameChanged(name);
    xChanged();
  }
}

void SvgAssembly::setSize(SvgAssemblySize size)
{
  if(_size != size)
  {
    _size = size;
    emit sizeChanged((int)size);
    xChanged();
  }
}

void SvgAssembly::setBackground(const QString& filePath)
{
  QString absoluteFilePath = _project->dir().absoluteFilePath(filePath);
  if(!filePath.isEmpty() && QFileInfo(absoluteFilePath).exists()
     && _background != absoluteFilePath)
  {
    QString file = absoluteFilePath;
    _background = _project->dir().relativeFilePath(absoluteFilePath);
    emit backgroundChanged(file);
    xChanged();
  }
  else if((filePath.isEmpty() || !QFileInfo(absoluteFilePath).exists())
          && !_background.isEmpty())
  {
    _background = "";
    emit backgroundChanged(_background);
    xChanged();
  }
}

void SvgAssembly::addElement(const QString& filePath, int index)
{
  QString absoluteFilePath = _project->dir().absoluteFilePath(filePath);
  if(!filePath.isEmpty() && QFileInfo(absoluteFilePath).exists())
  {
    QString relativeFile = _project->dir().relativeFilePath(absoluteFilePath);
    SvgAssemblyElement elem(relativeFile);
    elem.checkSize(_project->dir());
    if(xIndexValid(index))
    {
      _elements.insert(index, elem);
      emit elementAdded(absoluteFilePath, index);
    }
    else
    {
      _elements.push_back(elem);
      emit elementAdded(absoluteFilePath, _elements.count() - 1);
    }
    xChanged();
  }
}

void SvgAssembly::removeElement(int index)
{
  if(xIndexValid(index))
  {
    _elements.remove(index);
    emit elementRemoved(index);
    xChanged();
  }
}

void SvgAssembly::setElementScale(int index, qreal scale)
{
  if(xIndexValid(index) && _elements.at(index).scale != scale)
  {
    _elements[index].scale = scale;
    emit elementScaleChanged(index, scale);
    xChanged();
  }
}

void SvgAssembly::setElementDx(int index, qreal dx)
{
  if(xIndexValid(index) && _elements.at(index).dx != dx)
  {
    _elements[index].dx = dx;
    emit elementDxChanged(index, dx);
    xChanged();
  }
}

void SvgAssembly::setElementDy(int index, qreal dy)
{
  if(xIndexValid(index) && _elements.at(index).dy != dy)
  {
    _elements[index].dy = dy;
    emit elementDyChanged(index, dy);
    xChanged();
  }
}

void SvgAssembly::setElementPos(int index, qreal dx, qreal dy)
{
  if(xIndexValid(index) &&
     (_elements.at(index).dx != dx || _elements.at(index).dy != dy))
  {
    _elements[index].dx = dx;
    _elements[index].dy = dy;
    emit elementPosChanged(index, dx, dy);
    xChanged();
  }
}

void SvgAssembly::lowerElement(int index)
{
  if(xIndexValid(index) && index > 0)
  {
    SvgAssemblyElement tmp = _elements[index];
    _elements[index] = _elements[index-1];
    _elements[index-1] = tmp;
    emit elementLowered(index);
    xChanged();
  }
}

void SvgAssembly::raiseElement(int index)
{
  if(xIndexValid(index) && index+1 < _elements.count())
  {
    SvgAssemblyElement tmp = _elements[index];
    _elements[index] = _elements[index+1];
    _elements[index+1] = tmp;
    emit elementRaised(index);
    xChanged();
  }
}

void SvgAssembly::setElementHCenter(int index)
{
  if(xIndexValid(index) && _elements.at(index).validSize)
  {
    qreal dx;
    qreal sx = _elements.at(index).sx * _elements.at(index).scale;
    dx = ((int)_size - sx) / 2.0;
    if(_elements.at(index).dx != dx)
    {
      _elements[index].dx = dx;
      emit elementDxChanged(index, dx);
      xChanged();
    }
  }
}

void SvgAssembly::setElementVCenter(int index)
{
  if(xIndexValid(index) && _elements.at(index).validSize)
  {
    qreal dy;
    qreal sy = _elements.at(index).sy * _elements.at(index).scale;
    dy = ((int)_size - sy) / 2.0;
    if(_elements.at(index).dy != dy)
    {
      _elements[index].dy = dy;
      emit elementDyChanged(index, dy);
      xChanged();
    }
  }
}

void SvgAssembly::xChanged()
{
  emit assemblyChanged(xml());
  setHasChanged(true);
}

bool SvgAssembly::xIndexValid(int index) const
{
  return index >= 0 && index < _elements.count();
}

QDomDocumentFragment SvgAssembly::emptyFrag()
{
  return s_dummyDoc.createDocumentFragment();
}

QDomDocumentFragment SvgAssembly::toFrag(QDomElement elem)
{
  QDomDocumentFragment frag = s_dummyDoc.createDocumentFragment();
  frag.appendChild(elem.cloneNode().toElement());
  return frag;
}

} // namespace Composition
