#ifndef TREE_VIEW_CONTROLLER_HPP
#define TREE_VIEW_CONTROLLER_HPP

#include <QObject>
#include <QMap>

class QStandardItem;
class QStandardItemModel;

namespace SvgCompose
{
class SvgAssembliesList;
class SvgAssembly;
} // namespace SvgCompose

namespace SvgCompositor
{

class TreeViewController : public QObject
{
  Q_OBJECT
  typedef QMap<SvgCompose::SvgAssembly*, QStandardItem*> Link;

public:
  explicit TreeViewController(QObject *parent = NULL);

  QStandardItemModel* model() const;
  SvgCompose::SvgAssembliesList* project() const;

public Q_SLOTS:
  void onProjectChanged(SvgCompose::SvgAssembliesList* project);
  void onAssemblyChanged();
  void onAssemblyAdded(SvgCompose::SvgAssembly* assembly);
  void onAssemblyRemoved(SvgCompose::SvgAssembly* assembly);

  void onBackgroundChanged(const QString& background);
  void onElementAdded(const QString& file, int index);
  void onElementRemoved(int index);

Q_SIGNALS:
  void modelChanged(QStandardItemModel* model);

private:
  void assemblyConnectionSetup(SvgCompose::SvgAssembly* assembly);
  void assemblyConnectionTearDown(SvgCompose::SvgAssembly* assembly);

private:
  QStandardItemModel* _model;
  SvgCompose::SvgAssembliesList* _project;
  Link _link;
};

} // namespace SvgCompositor

#endif // TREE_VIEW_CONTROLLER_HPP
