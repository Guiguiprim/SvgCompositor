#ifndef COMPOSITOR_CONTROLLER_HPP
#define COMPOSITOR_CONTROLLER_HPP

#include <QObject>
#include <QMap>

namespace SvgCompose
{
class SvgAssembliesList;
class SvgAssembly;
} // namespace SvgCompose

namespace SvgCompositor
{
class Editor;

class CompositorController : public QObject
{
  Q_OBJECT
public:
  explicit CompositorController(QObject *parent = NULL);

  SvgCompose::SvgAssembliesList* project() const;

  void updateWindowTitle();

public Q_SLOTS:
  bool createProject();
  bool openProject(const QString& filename = QString());
  bool saveProjectAs();
  bool saveAssembly(SvgCompose::SvgAssembly* assembly);
  bool saveAll();
  bool closeProject();

  bool openAssembly(SvgCompose::SvgAssembly* assembly);
  bool showAssembly(SvgCompose::SvgAssembly* assembly);
  bool closeAssembly(SvgCompose::SvgAssembly* assembly);
  void setOutputDir(const QString& outputDir);

Q_SIGNALS:
  void setWindowTitle(const QString& title);
  void projectChanged(SvgCompose::SvgAssembliesList* _project);

  void addEditor(Editor*, const QString& name);
  void renameEditor(Editor*, const QString& name);
  void removeEditor(Editor*);
  void setCurrentEditor(Editor*);
  void outputDirChanged(const QString& outputDir);

private Q_SLOTS:
  void xOnAssemblyChanged();
  bool xCloseAssembly(QMap<SvgCompose::SvgAssembly*, Editor*>::iterator it, bool force = false);

private:
  SvgCompose::SvgAssembliesList* _project;
  QMap<SvgCompose::SvgAssembly*, Editor*> _editors;

public:
  static const QString k_windowTitle;
};

} // namespace SvgCompositor

#endif // COMPOSITOR_CONTROLLER_HPP
