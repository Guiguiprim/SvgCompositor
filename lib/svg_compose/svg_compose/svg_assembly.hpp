#ifndef SVG_ASSEMBLY_HPP
#define SVG_ASSEMBLY_HPP

#include <QObject>

#include <QDomElement>
#include <QSize>
#include <QVector>

#include <svg_compose/svg_assembly_element.hpp>
#include <svg_compose/svg_assembly_size.hpp>

namespace SvgCompose
{
class SvgAssembliesList;

struct SvgAssemblyData
{
  QString name;
  SvgAssemblySize size;
  QString background;
  QVector<SvgAssemblyElement> elements;

  SvgAssemblyData(QString n = "Untitled", SvgAssemblySize s = S48, QString b = QString());
  SvgAssemblyData(const SvgAssemblyData& d);
  const SvgAssemblyData& operator=(const SvgAssemblyData& d);
};

bool operator==(const SvgAssemblyData& d1, const SvgAssemblyData& d2);
bool operator!=(const SvgAssemblyData& d1, const SvgAssemblyData& d2);

class SvgAssembly : public QObject
{
  Q_OBJECT  
public:
  explicit SvgAssembly(SvgAssembliesList* project);
  explicit SvgAssembly(SvgAssembliesList* project, const QString& name);

  bool load(QDomElement& root);
  QDomElement createDom(bool lastSave = false) const;
  QString xml() const;

  QString name() const;
  int sizeInt() const;
  SvgAssemblySize size() const;
  QString background() const;
  SvgAssemblyElement element(int index) const;
  QVector<SvgAssemblyElement> elements() const;
  int elementsCount() const;
  SvgAssembliesList* project() const;

  void saveCurrentState();
  void resetLastState();
  bool hasChanged() const;

public Q_SLOTS:
  void setName(const QString& name);
  void setSize(SvgAssemblySize size);
  void setBackground(const QString& filePath);
  void addElement(const QString& filePath, int index = -1);
  void removeElement(int index);
  void setElementScale(int index, qreal scale);
  void setElementDx(int index, qreal dx);
  void setElementDy(int index, qreal dy);
  void setElementPos(int index, qreal dx, qreal dy);
  void lowerElement(int index);
  void raiseElement(int index);
  void setElementHCenter(int index); /// need element width
  void setElementVCenter(int index); /// need element height

Q_SIGNALS:
  void nameChanged(const QString& name);
  void sizeChanged(int size);
  void backgroundChanged(const QString& background);
  void elementAdded(const QString& file, int index = -1);
  void elementRemoved(int index);
  void elementScaleChanged(int index, qreal scale);
  void elementDxChanged(int index, qreal dx);
  void elementDyChanged(int index, qreal dy);
  void elementPosChanged(int index, qreal dx, qreal dy);
  void elementLowered(int index);
  void elementRaised(int index);

  void assemblyLoaded();
  void assemblyChanged(const QString& xml);
  void assemblyChanged(bool);

private:
  void xChanged();
  bool xIndexValid(int index) const;

private:
  SvgAssemblyData _data;
  SvgAssemblyData _lastSaveData;
  SvgAssembliesList* _project;
  bool _hasChanged;

  static QDomDocument s_dummyDoc;
public:
  static const QString k_tag;
  static QDomDocumentFragment emptyFrag();
  static QDomDocumentFragment toFrag(QDomElement elem);
};

} // namespace Composition

#endif // SVG_ASSEMBLY_HPP
