#ifndef SVG_UNION_H
#define SVG_UNION_H

class QDir;
class QDomElement;
class QString;

namespace SvgCompose
{
class SvgAssembliesList;
class SvgAssembly;
struct SvgAssemblyElement;

void generateAssemblies(
    const SvgAssembliesList* list);

bool generateAssembly(
    const SvgAssembly* assembly,
    const QDir& projectDir,
    const QDir& outputDir);

bool addBackgroundElement(
    QDomElement& svgRoot,
    const QDir& projectDir,
    const QString& file,
    int size);

bool addElement(
    QDomElement& svgRoot,
    const QDir& projectDir,
    const SvgAssemblyElement& element);

} // namespace AR

#endif // SVG_UNION_H
