#ifndef SVG_ASSEMBLIY_ELEMENT_HPP
#define SVG_ASSEMBLIY_ELEMENT_HPP

#include <QDir>
#include <QString>

namespace SvgCompose
{

struct SvgAssemblyElement {
  QString file;
  qreal scale;
  qreal dx;
  qreal dy;

  bool validSize;
  qreal sx;
  qreal sy;

  SvgAssemblyElement();
  SvgAssemblyElement(const QString& file);
  SvgAssemblyElement(const SvgAssemblyElement& u);
  SvgAssemblyElement& operator=(const SvgAssemblyElement& u);

  void checkSize(const QDir& projectDir);
};

bool operator==(const SvgAssemblyElement& u1, const SvgAssemblyElement& u2);
bool operator!=(const SvgAssemblyElement& u1, const SvgAssemblyElement& u2);

} // namespace Composition

#endif // SVG_ASSEMBLIY_ELEMENT_HPP
