#ifndef SVG_ASSEMBLIES_LIST_HPP
#define SVG_ASSEMBLIES_LIST_HPP

#include <QDir>
#include <QFileInfo>
#include <QMap>
#include <QObject>
#include <QVector>

namespace SvgCompose
{
class SvgAssembly;

class SvgAssembliesList : public QObject
{
  Q_OBJECT
public:
  explicit SvgAssembliesList(QObject *parent = 0);
  explicit SvgAssembliesList(const QString& filename, QObject *parent = 0);

  QString baseName() const;
  QString fileName() const;
  QString filePath() const;
  QDir dir() const;
  QDir outputDir() const;

  SvgAssembly* assembly(int index) const;
  QVector<SvgAssembly*> assemblies() const;
  int count() const;

  int indexOf(SvgAssembly* assembly) const;

  void setFileName(const QString& filename);
  void setOutputDir(const QString& path);

  bool load(const QString& filename = QString());
  bool save(const QString& filename = QString());

  bool hasChanged() const;

public Q_SLOTS:
  SvgAssembly* createNew();
  void saveAssembly(SvgCompose::SvgAssembly* assembly);
  void saveAll();

Q_SIGNALS:
  void assemblyAdded(SvgCompose::SvgAssembly* assembly);
  void assemblyRemoved(SvgCompose::SvgAssembly* assembly);
  void outputDirChanged(const QString& outputDir);

private:
  QFileInfo _fileInfo;
  QString _outputDir;
  QVector<SvgAssembly*> _assemblies;
};

} // namespace Composition

#endif // SVG_ASSEMBLIES_LIST_HPP
