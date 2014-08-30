#ifndef SVG_UNION_H
#define SVG_UNION_H

#include <QString>

class QDir;
class QDomElement;

namespace SvgCompose
{
class SvgAssembliesList;
class SvgAssembly;
struct SvgAssemblyElement;

/**
 * @brief generateAssemblies
 * @param list: the project to generate
 * @param outputDir: Output folder (will overright the project output folder)
 */
void generateAssemblies(
    const SvgAssembliesList* list,
    const QString& outputDir = QString());

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
