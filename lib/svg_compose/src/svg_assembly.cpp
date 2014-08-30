#include <svg_compose/svg_assembly.hpp>

#include <QDir>
#include <QFileInfo>
#include <QTextStream>

#include <svg_compose/svg_assemblies_list.hpp>

namespace SvgCompose
{
SvgAssemblyData::SvgAssemblyData(QString n, SvgAssemblySize s, QString b)
  : name(n)
  , size(s)
  , background(b)
  , elements()
{}

SvgAssemblyData::SvgAssemblyData(const SvgAssemblyData& d)
  : name(d.name)
  , size(d.size)
  , background(d.background)
  , elements(d.elements)
{}

const SvgAssemblyData& SvgAssemblyData::operator=(const SvgAssemblyData& d)
{
  name = d.name;
  size = d.size;
  background = d.background;
  elements = d.elements;
  return *this;
}

bool operator==(const SvgAssemblyData& d1, const SvgAssemblyData& d2)
{
  if(d1.name != d2.name)
    return false;
  if(d1.size != d2.size)
    return false;
  if(d1.background != d2.background)
    return false;
  if(d1.elements.size() != d2.elements.size())
    return false;
  for(int i=0; i<d1.elements.size(); ++i)
  {
    if(d1.elements.at(i) != d2.elements.at(i))
      return false;
  }
  return true;
}

bool operator!=(const SvgAssemblyData& d1, const SvgAssemblyData& d2)
{
  return !(d1 == d2);
}

QDomDocument SvgAssembly::s_dummyDoc = QDomDocument();
const QString SvgAssembly::k_tag = "assembly";

SvgAssembly::SvgAssembly(SvgAssembliesList* project)
  : QObject(project)
  , _data("Untitled", S48, "")
  , _lastSaveData(_data)
  , _project(project)
  , _hasChanged(false)
{
}

SvgAssembly::SvgAssembly(SvgAssembliesList* project, const QString& name)
  : QObject(project)
  , _data(name, S48, "")
  , _lastSaveData(_data)
  , _project(project)
  , _hasChanged(false)
{
}

QString SvgAssembly::name() const
{
  return _data.name;
}

int SvgAssembly::sizeInt() const
{
  return (int)_data.size;
}

SvgAssemblySize SvgAssembly::size() const
{
  return _data.size;
}

QString SvgAssembly::background() const
{
  return _data.background;
}

SvgAssemblyElement SvgAssembly::element(int index) const
{
  if(xIndexValid(index))
    return _data.elements.at(index);
  return SvgAssemblyElement();
}

QVector<SvgAssemblyElement> SvgAssembly::elements() const
{
  return _data.elements;
}

int SvgAssembly::elementsCount() const
{
  return _data.elements.count();
}

SvgAssembliesList* SvgAssembly::project() const
{
  return _project;
}

void SvgAssembly::saveCurrentState()
{
  _lastSaveData = _data;
  if(_hasChanged != false)
  {
    _hasChanged = false;
    Q_EMIT assemblyChanged(_hasChanged);
  }
}

void SvgAssembly::resetLastState()
{
  _data = _lastSaveData;
  if(_hasChanged != false)
  {
    _hasChanged = false;
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

  _data.name = name;
  _data.size = toComposeSize(size);
  _data.background = background;
  _data.elements = unions;

  _lastSaveData = _data;
  _hasChanged = false;
  emit assemblyLoaded();
  return true;
}

QDomElement SvgAssembly::createDom(bool lastSave) const
{
  const SvgAssemblyData* data;
  if(lastSave)
    data = &_lastSaveData;
  else
    data = &_data;
  QDomElement root = s_dummyDoc.createElement(k_tag);
  root.setAttribute("name", data->name);
  root.setAttribute("size", (int)data->size);

  if(!data->background.isEmpty())
  {
      QDomElement backElem = s_dummyDoc.createElement("background");
      backElem.setAttribute("path", data->background);
      root.appendChild(backElem);
  }

  Q_FOREACH(SvgAssemblyElement uElem, data->elements)
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
  return root;
}

QString SvgAssembly::xml() const
{
  QString str;
  QTextStream stream(&str);
  QDomElement elem = createDom();
  elem.save(stream, 2);
  return str;
}

void SvgAssembly::setName(const QString& name)
{
  if(!name.isEmpty() && _data.name != name)
  {
    _data.name = name;
    emit nameChanged(name);
    xChanged();
  }
}

void SvgAssembly::setSize(SvgAssemblySize size)
{
  if(_data.size != size)
  {
    SvgAssemblySize old = _data.size;
    _data.size = size;
    emit sizeChanged((int)size);
    qreal fac = (qreal)size / (qreal) old;
    for(int i=0; i < _data.elements.size(); ++i)
    {
      qreal x = _data.elements.at(i).dx * fac;
      qreal y = _data.elements.at(i).dy * fac;
      qreal s = _data.elements.at(i).scale * fac;
      setElementScale(i, s);
      setElementPos(i, x, y);
    }
    xChanged();
  }
}

void SvgAssembly::setBackground(const QString& filePath)
{
  QString absoluteFilePath = _project->dir().absoluteFilePath(filePath);
  if(!filePath.isEmpty() && QFileInfo(absoluteFilePath).exists()
     && _data.background != absoluteFilePath)
  {
    QString file = absoluteFilePath;
    _data.background = _project->dir().relativeFilePath(absoluteFilePath);
    emit backgroundChanged(file);
    xChanged();
  }
  else if((filePath.isEmpty() || !QFileInfo(absoluteFilePath).exists())
          && !_data.background.isEmpty())
  {
    _data.background = "";
    emit backgroundChanged(_data.background);
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
      _data.elements.insert(index, elem);
      emit elementAdded(absoluteFilePath, index);
    }
    else
    {
      _data.elements.push_back(elem);
      emit elementAdded(absoluteFilePath, _data.elements.count() - 1);
    }
    xChanged();
  }
}

void SvgAssembly::removeElement(int index)
{
  if(xIndexValid(index))
  {
    _data.elements.remove(index);
    emit elementRemoved(index);
    xChanged();
  }
}

void SvgAssembly::setElementScale(int index, qreal scale)
{
  if(xIndexValid(index) && _data.elements.at(index).scale != scale)
  {
    _data.elements[index].scale = scale;
    emit elementScaleChanged(index, scale);
    xChanged();
  }
}

void SvgAssembly::setElementDx(int index, qreal dx)
{
  if(xIndexValid(index) && _data.elements.at(index).dx != dx)
  {
    _data.elements[index].dx = dx;
    emit elementDxChanged(index, dx);
    xChanged();
  }
}

void SvgAssembly::setElementDy(int index, qreal dy)
{
  if(xIndexValid(index) && _data.elements.at(index).dy != dy)
  {
    _data.elements[index].dy = dy;
    emit elementDyChanged(index, dy);
    xChanged();
  }
}

void SvgAssembly::setElementPos(int index, qreal dx, qreal dy)
{
  if(xIndexValid(index) &&
     (_data.elements.at(index).dx != dx || _data.elements.at(index).dy != dy))
  {
    _data.elements[index].dx = dx;
    _data.elements[index].dy = dy;
    emit elementPosChanged(index, dx, dy);
    xChanged();
  }
}

void SvgAssembly::lowerElement(int index)
{
  if(xIndexValid(index) && index > 0)
  {
    SvgAssemblyElement tmp = _data.elements[index];
    _data.elements[index] = _data.elements[index-1];
    _data.elements[index-1] = tmp;
    emit elementLowered(index);
    xChanged();
  }
}

void SvgAssembly::raiseElement(int index)
{
  if(xIndexValid(index) && index+1 < _data.elements.count())
  {
    SvgAssemblyElement tmp = _data.elements[index];
    _data.elements[index] = _data.elements[index+1];
    _data.elements[index+1] = tmp;
    emit elementRaised(index);
    xChanged();
  }
}

void SvgAssembly::setElementHCenter(int index)
{
  if(xIndexValid(index) && _data.elements.at(index).validSize)
  {
    qreal dx;
    qreal sx = _data.elements.at(index).sx * _data.elements.at(index).scale;
    dx = ((int)_data.size - sx) / 2.0;
    if(_data.elements.at(index).dx != dx)
    {
      _data.elements[index].dx = dx;
      emit elementDxChanged(index, dx);
      xChanged();
    }
  }
}

void SvgAssembly::setElementVCenter(int index)
{
  if(xIndexValid(index) && _data.elements.at(index).validSize)
  {
    qreal dy;
    qreal sy = _data.elements.at(index).sy * _data.elements.at(index).scale;
    dy = ((int)_data.size - sy) / 2.0;
    if(_data.elements.at(index).dy != dy)
    {
      _data.elements[index].dy = dy;
      emit elementDyChanged(index, dy);
      xChanged();
    }
  }
}

void SvgAssembly::xChanged()
{
  emit assemblyChanged(xml());
  bool hasChanged = !(_data == _lastSaveData);
  if(_hasChanged != hasChanged)
  {
    _hasChanged = hasChanged;
    Q_EMIT assemblyChanged(_hasChanged);
  }
}

bool SvgAssembly::xIndexValid(int index) const
{
  return index >= 0 && index < _data.elements.count();
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
