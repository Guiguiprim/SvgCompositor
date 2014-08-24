#include "tree_view_controller.hpp"

#include <QStandardItem>
#include <QStandardItemModel>

#include <svg_compose/svg_assemblies_list.hpp>
#include <svg_compose/svg_assembly.hpp>
#include <svg_compose/svg_assembly_element.hpp>

namespace SvgCompositor {

TreeViewController::TreeViewController(QObject *parent)
  : QObject(parent)
  , _model(NULL)
  , _project(NULL)
  , _link()
{
}

QStandardItemModel* TreeViewController::model() const
{
  return _model;
}

SvgCompose::SvgAssembliesList* TreeViewController::project() const
{
  return _project;
}

void TreeViewController::onProjectChanged(
    SvgCompose::SvgAssembliesList* project)
{
  if(_project)
  {
    Link::iterator it, end;
    for(it = _link.begin(), end = _link.end(); it != end; ++it)
    {
      xAssemblyConnectionTearDown(it.key());
    }
    _link.clear();
    disconnect(_project, SIGNAL(assemblyAdded(SvgCompose::SvgAssembly*)),
               this, SLOT(onAssemblyAdded(SvgCompose::SvgAssembly*)));
    disconnect(_project, SIGNAL(assemblyRemoved(SvgCompose::SvgAssembly*)),
               this, SLOT(onAssemblyRemoved(SvgCompose::SvgAssembly*)));
    delete _model;
    _model = NULL;
    Q_EMIT modelChanged(NULL);
  }
  _project = project;
  if(_project)
  {
    _model = new QStandardItemModel(this);
    QStandardItem* root = _model->invisibleRootItem();
    QFont font;
    font.setItalic(true);
    Q_FOREACH(SvgCompose::SvgAssembly* assembly, _project->assemblies())
    {
      QStandardItem* item = new QStandardItem(assembly->name());
      item->setEditable(false);
      root->appendRow(item);
      _link[assembly] = item;
      xAssemblyConnectionSetup(assembly);

      QString imgFile;
      QStandardItem* subitem;
      if(assembly->background().isEmpty())
        subitem = new QStandardItem("No background");
      else
      {
        imgFile = _project->dir().absoluteFilePath(assembly->background());
        subitem = new QStandardItem(QIcon(imgFile), assembly->background());
      }
      subitem->setEditable(false);
      subitem->setFont(font);
      item->appendRow(subitem);
      Q_FOREACH(SvgCompose::SvgAssemblyElement element, assembly->elements())
      {
        imgFile = _project->dir().absoluteFilePath(element.file);
        subitem = new QStandardItem(QIcon(imgFile), element.file);
        subitem->setEditable(false);
        item->appendRow(subitem);
      }
    }
    connect(_project, SIGNAL(assemblyAdded(SvgCompose::SvgAssembly*)),
            this, SLOT(onAssemblyAdded(SvgCompose::SvgAssembly*)));
    connect(_project, SIGNAL(assemblyRemoved(SvgCompose::SvgAssembly*)),
            this, SLOT(onAssemblyRemoved(SvgCompose::SvgAssembly*)));
    Q_EMIT modelChanged(_model);
  }
}

void TreeViewController::onAssemblyChanged()
{
  SvgCompose::SvgAssembly* assembly = qobject_cast<SvgCompose::SvgAssembly*>(sender());
  if(assembly)
  {
    Link::iterator it = _link.find(assembly);
    if(it != _link.end())
    {
      it.value()->setData(it.key()->hasChanged() ? "*" + it.key()->name() : it.key()->name(),
                          Qt::DisplayRole);
      QFont font = it.value()->font();
      font.setBold(it.key()->hasChanged());
      it.value()->setFont(font);
    }
  }
}

void TreeViewController::onAssemblyAdded(SvgCompose::SvgAssembly* assembly)
{
  if(_project->assemblies().contains(assembly))
  {
    QStandardItem* root = _model->invisibleRootItem();
    QStandardItem* item = new QStandardItem(assembly->name());
    root->appendRow(root);
    _link[assembly] = item;
    xAssemblyConnectionSetup(assembly);
  }
}

void TreeViewController::onAssemblyRemoved(SvgCompose::SvgAssembly* assembly)
{
  Link::iterator it = _link.find(assembly);
  if(it != _link.end())
  {
    xAssemblyConnectionTearDown(it.key());
    QStandardItem* root = _model->invisibleRootItem();
    root->removeRow(it.value()->row());
    _link.remove(it.key());
  }
}

void TreeViewController::onBackgroundChanged(const QString& background)
{
  SvgCompose::SvgAssembly* assembly = qobject_cast<SvgCompose::SvgAssembly*>(sender());
  if(assembly)
  {
    Link::iterator it = _link.find(assembly);
    if(it != _link.end())
    {
      QStandardItem* item = it.value()->child(0);
      if(background.isEmpty())
      {
        item->setData("No background", Qt::DisplayRole);
        item->setData(QVariant(), Qt::DecorationRole);
      }
      else
      {
        QString imgFile = _project->dir().absoluteFilePath(background);
        QString name = _project->dir().relativeFilePath(background);
        item->setData(name, Qt::DisplayRole);
        item->setData(QIcon(imgFile), Qt::DecorationRole);
      }
    }
  }
}

void TreeViewController::onElementAdded(const QString& file, int index)
{
  SvgCompose::SvgAssembly* assembly = qobject_cast<SvgCompose::SvgAssembly*>(sender());
  if(assembly)
  {
    Link::iterator it = _link.find(assembly);
    if(it != _link.end())
    {
      QIcon icon;
      QString name;
      if(!file.isEmpty())
      {
        QString imgFile = _project->dir().absoluteFilePath(file);
        if(QFileInfo(imgFile).exists())
          icon = QIcon(imgFile);
        name = _project->dir().relativeFilePath(file);
      }
      QStandardItem* item = new QStandardItem(icon, name);
      it.value()->insertRow(index +1, item); // +1 because of backgground
    }
  }
}

void TreeViewController::onElementRemoved(int index)
{
  SvgCompose::SvgAssembly* assembly = qobject_cast<SvgCompose::SvgAssembly*>(sender());
  if(assembly)
  {
    Link::iterator it = _link.find(assembly);
    if(it != _link.end())
    {
      it.value()->removeRow(index +1); // +1 because of backgground
    }
  }
}

void TreeViewController::onClicked(const QModelIndex& index)
{
  QStandardItem* item = _model->itemFromIndex(index);
  SvgCompose::SvgAssembly* assembly = _link.key(item, NULL);
  if(assembly)
    Q_EMIT showAssembly(assembly);
}

void TreeViewController::onDoubleClicked(const QModelIndex& index)
{
  QStandardItem* item = _model->itemFromIndex(index);
  SvgCompose::SvgAssembly* assembly = _link.key(item, NULL);
  if(assembly)
    Q_EMIT openAssembly(assembly);
}

void TreeViewController::xAssemblyConnectionSetup(SvgCompose::SvgAssembly* assembly)
{
  connect(assembly, SIGNAL(nameChanged(QString)),
          this, SLOT(onAssemblyChanged()));
  connect(assembly, SIGNAL(assemblyChanged(bool)),
          this, SLOT(onAssemblyChanged()));
  connect(assembly, SIGNAL(backgroundChanged(QString)),
          this, SLOT(onBackgroundChanged(QString)));
  connect(assembly, SIGNAL(elementAdded(QString,int)),
          this, SLOT(onElementAdded(QString,int)));
  connect(assembly, SIGNAL(elementRemoved(int)),
          this, SLOT(onElementRemoved(int)));
}

void TreeViewController::xAssemblyConnectionTearDown(SvgCompose::SvgAssembly* assembly)
{
  disconnect(assembly, SIGNAL(nameChanged(QString)),
             this, SLOT(onAssemblyChanged()));
  disconnect(assembly, SIGNAL(assemblyChanged(bool)),
             this, SLOT(onAssemblyChanged()));
  disconnect(assembly, SIGNAL(backgroundChanged(QString)),
             this, SLOT(onBackgroundChanged(QString)));
  disconnect(assembly, SIGNAL(elementAdded(QString,int)),
             this, SLOT(onElementAdded(QString,int)));
  disconnect(assembly, SIGNAL(elementRemoved(int)),
             this, SLOT(onElementRemoved(int)));
}

} // namespace SvgCompositor
