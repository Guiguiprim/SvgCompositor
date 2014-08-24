#ifndef COMPOSITOR_CONTROLLER_HPP
#define COMPOSITOR_CONTROLLER_HPP

#include <QObject>
#include <QMap>

class QUndoGroup;
class QWidget;

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
  explicit CompositorController(QWidget *parent = NULL);

  SvgCompose::SvgAssembliesList* project() const;
  QUndoGroup* undoGroup() const;

  void updateWindowTitle();

public Q_SLOTS:
  bool createProject();
  bool openProject(const QString& filename = QString());
  bool saveProjectAs();
  bool saveAssembly(SvgCompose::SvgAssembly* assembly);
  bool saveAll();
  bool closeProject(bool force = false);

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
  bool xCloseAssembly(SvgCompose::SvgAssembly* assembly, Editor* editor, bool force = false);

private:
  QWidget* _parentWidget;
  SvgCompose::SvgAssembliesList* _project;
  QMap<SvgCompose::SvgAssembly*, Editor*> _editors;
  QUndoGroup* _undoGroup;

public:
  static const QString k_windowTitle;
};

} // namespace SvgCompositor

#endif // COMPOSITOR_CONTROLLER_HPP
