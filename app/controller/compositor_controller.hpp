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

public Q_SLOTS:
  bool openProject(const QString& filename);
  bool saveProjectAs(const QString& filename = QString());
  bool saveAssembly(SvgCompose::SvgAssembly* assembly);
  bool saveAllProject();
  bool closeProject();

  bool openAssembly(SvgCompose::SvgAssembly* assembly);
  bool closeAssembly(SvgCompose::SvgAssembly* assembly);

Q_SIGNALS:
  void projectChanged(SvgCompose::SvgAssembliesList* _project);

  void addEditor(Editor*, const QString& name);
  void renameEditor(Editor*, const QString& name);
  void removeEditor(Editor*);
  void setCurrentEditor(Editor*);

private Q_SLOTS:
  void xOnAssemblyNameChanged(const QString& name);
  bool xCloseAssembly(QMap<SvgCompose::SvgAssembly*, Editor*>::iterator it, bool force = false);

private:
  SvgCompose::SvgAssembliesList* _project;
  QMap<SvgCompose::SvgAssembly*, Editor*> _editors;
};

} // namespace SvgCompositor

#endif // COMPOSITOR_CONTROLLER_HPP
