#ifndef TREE_VIEW_CONTROLLER_HPP
#define TREE_VIEW_CONTROLLER_HPP

#include <QObject>
#include <QMap>
#include <QModelIndex>

#include "misc/project_model_enum.hpp"

class QAction;
class QPoint;
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

  struct ItemSelected
  {
    ItemType type;
    SvgCompose::SvgAssembly* assembly;
    int index;
    bool isOpen;
    ItemSelected() : type(AssemblyType) , assembly(NULL), index(-1), isOpen(false) {}
  };

public:
  explicit TreeViewController(QObject *parent = NULL);

  QStandardItemModel* model() const;
  SvgCompose::SvgAssembliesList* project() const;

public Q_SLOTS:
  void onProjectChanged(SvgCompose::SvgAssembliesList* project);

  void onClicked(const QModelIndex& index);
  void onDoubleClicked(const QModelIndex& index);
  void customMenuRequested(const QModelIndex& index, const QPoint& pos);

  void onGenerateImageTriggered();
  void onDeleteTriggered();

  void onAssemblyOpenStatusChanged(SvgCompose::SvgAssembly* assembly, bool open);

private Q_SLOTS:
  void xOnAssemblyAdded(SvgCompose::SvgAssembly* assembly);
  void xOnAssemblyRemoved(SvgCompose::SvgAssembly* assembly);

  void xOnAssemblyChanged();
  void xOnBackgroundChanged(const QString& background);
  void xOnElementAdded(const QString& file, int index);
  void xOnElementRemoved(int index);
  void xOnElementLowered(int index);
  void xOnElementRaised(int index);

  void xOnOpenTriggered();

Q_SIGNALS:
  void modelChanged(QStandardItemModel* model);
  void openAssembly(SvgCompose::SvgAssembly* assembly);
  void showAssembly(SvgCompose::SvgAssembly* assembly);
  void removeAssembly(SvgCompose::SvgAssembly* assembly);
  void generateImage(SvgCompose::SvgAssembly* assembly);

  void enableAssemblyActions(bool enable);

private:
  void xAddAssembly(SvgCompose::SvgAssembly* assembly, QStandardItem* root);
  void xAddBackground(const QString& background, QStandardItem* assItem);
  void xAddElement(const QString& file, int index, QStandardItem* assItem);
  void xMoveElement(int oldindex, int newIndex, QStandardItem* assItem);

  void xAssemblyConnectionSetup(SvgCompose::SvgAssembly* assembly);
  void xAssemblyConnectionTearDown(SvgCompose::SvgAssembly* assembly);

  bool xSetLastItemSelected(const QModelIndex& index);

private:
  QStandardItemModel* _model;
  SvgCompose::SvgAssembliesList* _project;
  Link _link;
  QAction* _openAction;
  QAction* _deleteAction;
  ItemSelected _lastItemSelected;
};

} // namespace SvgCompositor

#endif // TREE_VIEW_CONTROLLER_HPP
