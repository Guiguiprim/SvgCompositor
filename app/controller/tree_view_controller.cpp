#include "tree_view_controller.hpp"

#include <QAction>
#include <QMenu>
#include <QPoint>
#include <QStandardItem>
#include <QStandardItemModel>

#include <svg_compose/svg_assemblies_list.hpp>
#include <svg_compose/svg_assembly.hpp>
#include <svg_compose/svg_assembly_element.hpp>

namespace SvgCompositor
{

TreeViewController::TreeViewController(QObject *parent)
  : QObject(parent)
  , _model(NULL)
  , _project(NULL)
  , _link()
  , _lastItemSelected()
{
  _openAction = new QAction("Open", this);
  QFont font = _openAction->font();
  font.setBold(true);
  _openAction->setFont(font);
  connect(_openAction, SIGNAL(triggered())
          , this, SLOT(xOnOpenTriggered()));

  _deleteAction = new QAction("Delete", this);
  connect(_deleteAction, SIGNAL(triggered())
          , this, SLOT(onDeleteTriggered()));

  _chooseBackgroundAction = new QAction("Choose a background", this);
  connect(_chooseBackgroundAction, SIGNAL(triggered())
          , this, SLOT(xOnChooseBackTriggered()));

  _removeBackgroundAction = new QAction("Remove", this);
  connect(_removeBackgroundAction, SIGNAL(triggered())
          , this, SLOT(xOnRemoveBackTriggered()));

  _selectElementAction = new QAction("Select", this);
  _selectElementAction->setFont(font);
  connect(_selectElementAction, SIGNAL(triggered())
          , this, SLOT(xOnSelectElementTriggered()));

  _removeElementAction = new QAction("Remove", this);
  connect(_removeElementAction, SIGNAL(triggered())
          , this, SLOT(xOnRemoveElementTriggered()));
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
               this, SLOT(xOnAssemblyAdded(SvgCompose::SvgAssembly*)));
    disconnect(_project, SIGNAL(assemblyRemoved(SvgCompose::SvgAssembly*)),
               this, SLOT(xOnAssemblyRemoved(SvgCompose::SvgAssembly*)));
    delete _model;
    _model = NULL;
    _lastItemSelected = ItemSelected();
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
      xAddAssembly(assembly, root);
    }
    connect(_project, SIGNAL(assemblyAdded(SvgCompose::SvgAssembly*)),
            this, SLOT(xOnAssemblyAdded(SvgCompose::SvgAssembly*)));
    connect(_project, SIGNAL(assemblyRemoved(SvgCompose::SvgAssembly*)),
            this, SLOT(xOnAssemblyRemoved(SvgCompose::SvgAssembly*)));
    Q_EMIT modelChanged(_model);
  }
}

void TreeViewController::onClicked(const QModelIndex& index)
{
  xSetLastItemSelected(index);
  if(_lastItemSelected.type == AssemblyType)
    Q_EMIT showAssembly(_lastItemSelected.assembly);
}

void TreeViewController::onDoubleClicked(const QModelIndex& index)
{
  xSetLastItemSelected(index);
  if(_lastItemSelected.type == AssemblyType)
    Q_EMIT openAssembly(_lastItemSelected.assembly);
  else if(_lastItemSelected.type == ElementType && _lastItemSelected.isOpen)
    xOnSelectElementTriggered();
}

void TreeViewController::customMenuRequested(const QModelIndex& index, const QPoint& pos)
{
  xSetLastItemSelected(index);
  QList<QAction*> actions;

  if(_lastItemSelected.type == AssemblyType)
    actions << _openAction << _deleteAction;

  if(_lastItemSelected.type == BackgroundType && _lastItemSelected.isOpen)
  {
    actions << _chooseBackgroundAction;
    if(index.data(BackgroundRole).toBool())
      actions << _removeBackgroundAction;
  }

  if(_lastItemSelected.type == ElementType && _lastItemSelected.isOpen)
    actions << _selectElementAction << _removeElementAction;

  if(actions.count() > 0)
    QMenu::exec(actions, pos);
}


void TreeViewController::onGenerateImageTriggered()
{
  if(_lastItemSelected.type == AssemblyType)
    Q_EMIT generateImage(_lastItemSelected.assembly);
}

void TreeViewController::onDeleteTriggered()
{
  if(_lastItemSelected.type == AssemblyType)
    Q_EMIT removeAssembly(_lastItemSelected.assembly);
}

void TreeViewController::onAssemblyOpenStatusChanged(SvgCompose::SvgAssembly* assembly, bool open)
{
  Link::iterator it = _link.find(assembly);
  if(it != _link.end())
  {
    it.value()->setData(open, OpenRole);
    QFont font = it.value()->font();
    font.setItalic(!open);
    it.value()->setFont(font);
  }
}

void TreeViewController::xOnAssemblyAdded(SvgCompose::SvgAssembly* assembly)
{
  if(_project->assemblies().contains(assembly))
  {
    QStandardItem* root = _model->invisibleRootItem();
    xAddAssembly(assembly, root);
  }
}

void TreeViewController::xOnAssemblyRemoved(SvgCompose::SvgAssembly* assembly)
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

void TreeViewController::xOnAssemblyChanged()
{
  SvgCompose::SvgAssembly* assembly = qobject_cast<SvgCompose::SvgAssembly*>(sender());
  if(assembly)
  {
    Link::iterator it = _link.find(assembly);
    if(it != _link.end())
    {
      it.value()->setData(it.key()->hasChanged() ?  it.key()->name() + "*" : it.key()->name(),
                          Qt::DisplayRole);
      QFont font = it.value()->font();
      font.setBold(it.key()->hasChanged());
      it.value()->setFont(font);
    }
  }
}

void TreeViewController::xOnBackgroundChanged(const QString& background)
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
        backItem->setData(false, BackgroundRole);
      }
      else
      {
        QString imgFile = _project->dir().absoluteFilePath(background);
        QString name = _project->dir().relativeFilePath(background);
        item->setData(name, Qt::DisplayRole);
        item->setData(QIcon(imgFile), Qt::DecorationRole);
        backItem->setData(true, BackgroundRole);
      }
    }
  }
}

void TreeViewController::xOnElementAdded(const QString& file, int index)
{
  SvgCompose::SvgAssembly* assembly = qobject_cast<SvgCompose::SvgAssembly*>(sender());
  if(assembly)
  {
    Link::iterator it = _link.find(assembly);
    if(it != _link.end())
    {
      xAddElement(file, index, it.value());
    }
  }
}

void TreeViewController::xOnElementRemoved(int index)
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

void TreeViewController::xOnElementLowered(int index)
{
  SvgCompose::SvgAssembly* assembly = qobject_cast<SvgCompose::SvgAssembly*>(sender());
  if(assembly)
  {
    Link::iterator it = _link.find(assembly);
    if(it != _link.end())
    {
      xMoveElement(index, index-1, it.value());
    }
  }
}

void TreeViewController::xOnElementRaised(int index)
{
  SvgCompose::SvgAssembly* assembly = qobject_cast<SvgCompose::SvgAssembly*>(sender());
  if(assembly)
  {
    Link::iterator it = _link.find(assembly);
    if(it != _link.end())
    {
      xMoveElement(index, index+1, it.value());
    }
  }
}

void TreeViewController::xOnOpenTriggered()
{
  if(_lastItemSelected.type == AssemblyType)
    Q_EMIT openAssembly(_lastItemSelected.assembly);
}

void TreeViewController::xOnChooseBackTriggered()
{
  if(_lastItemSelected.type == BackgroundType &&
     _lastItemSelected.isOpen)
  {
    Q_EMIT assemblyAction(_lastItemSelected.assembly,
                          SetBackground);
  }
}

void TreeViewController::xOnRemoveBackTriggered()
{
  if(_lastItemSelected.type == BackgroundType &&
     _lastItemSelected.isOpen)
  {
    Q_EMIT assemblyAction(_lastItemSelected.assembly,
                          RemoveBackground);
  }
}

void TreeViewController::xOnSelectElementTriggered()
{
  if(_lastItemSelected.type == ElementType &&
     _lastItemSelected.isOpen &&
     _lastItemSelected.index != -1)
  {
    Q_EMIT selectElement(_lastItemSelected.assembly,
                          _lastItemSelected.index);
  }
}

void TreeViewController::xOnRemoveElementTriggered()
{
  if(_lastItemSelected.type == ElementType &&
     _lastItemSelected.isOpen &&
     _lastItemSelected.index != -1)
  {
    Q_EMIT assemblyAction(_lastItemSelected.assembly,
                          Remove,
                          _lastItemSelected.index);
  }
}

void TreeViewController::xAddAssembly(SvgCompose::SvgAssembly* assembly, QStandardItem* root)
{
  QStandardItem* item = new QStandardItem(assembly->name());
  item->setData(AssemblyType, TypeRole);
  item->setEditable(false);

  item->setData(false, OpenRole);
  QFont font = item->font();
  font.setItalic(true);
  item->setFont(font);

  root->appendRow(item);
  _link[assembly] = item;
  xAssemblyConnectionSetup(assembly);

  xAddBackground(assembly->background(), item);
  for(int i=0; i<assembly->elementsCount(); ++i)
  {
    xAddElement(assembly->element(i).file, i, item);
  }
}

void TreeViewController::xAddBackground(const QString& background, QStandardItem* assItem)
{
  QFont font;
  font.setItalic(true);
  QStandardItem* backItem;
  if(background.isEmpty())
  {
    backItem = new QStandardItem("No background");
    backItem->setData(false, BackgroundRole);
  }
  else
  {
    QString imgFile = _project->dir().absoluteFilePath(background);
    QString name = _project->dir().relativeFilePath(background);
    QIcon icon;
    if(QFileInfo(imgFile).exists())
      icon = QIcon(imgFile);
    // TODO else icon = Icon::InvalideFile
    backItem = new QStandardItem(icon, name);
    backItem->setData(true, BackgroundRole);
  }
  backItem->setData(BackgroundType, TypeRole);
  backItem->setEditable(false);
  backItem->setSelectable(false);
  backItem->setFont(font);
  assItem->appendRow(backItem);
}

void TreeViewController::xAddElement(const QString& file, int index, QStandardItem* assItem)
{
  QString imgFile = _project->dir().absoluteFilePath(file);
  QString name = _project->dir().relativeFilePath(file);
  QIcon icon;
  if(QFileInfo(imgFile).exists())
    icon = QIcon(imgFile);
  // TODO else icon = Icon::InvalideFile
  QStandardItem* elemItem = new QStandardItem(icon, name);
  elemItem->setData(ElementType, TypeRole);
  elemItem->setEditable(false);
  elemItem->setSelectable(false);
  assItem->insertRow(index +1, elemItem); // +1 because of backgground
}

void TreeViewController::xMoveElement(int oldindex, int newIndex, QStandardItem* assItem)
{
  QStandardItem* oldItem = assItem->child(oldindex + 1); // +1 because of backgground
  if(!oldItem)
    return;
  QStandardItem* newItem = oldItem->clone();
  assItem->removeRow(oldindex + 1);
  assItem->insertRow(newIndex + 1, newItem); // +1 because of backgground
}

void TreeViewController::xAssemblyConnectionSetup(SvgCompose::SvgAssembly* assembly)
{
  connect(assembly, SIGNAL(nameChanged(QString)),
          this, SLOT(xOnAssemblyChanged()));
  connect(assembly, SIGNAL(assemblyChanged(bool)),
          this, SLOT(xOnAssemblyChanged()));
  connect(assembly, SIGNAL(backgroundChanged(QString)),
          this, SLOT(xOnBackgroundChanged(QString)));
  connect(assembly, SIGNAL(elementAdded(QString,int)),
          this, SLOT(xOnElementAdded(QString,int)));
  connect(assembly, SIGNAL(elementRemoved(int)),
          this, SLOT(xOnElementRemoved(int)));
  connect(assembly, SIGNAL(elementLowered(int)),
          this, SLOT(xOnElementLowered(int)));
  connect(assembly, SIGNAL(elementRaised(int)),
          this, SLOT(xOnElementRaised(int)));
}

void TreeViewController::xAssemblyConnectionTearDown(SvgCompose::SvgAssembly* assembly)
{
  disconnect(assembly, SIGNAL(nameChanged(QString)),
             this, SLOT(xOnAssemblyChanged()));
  disconnect(assembly, SIGNAL(assemblyChanged(bool)),
             this, SLOT(xOnAssemblyChanged()));
  disconnect(assembly, SIGNAL(backgroundChanged(QString)),
             this, SLOT(xOnBackgroundChanged(QString)));
  disconnect(assembly, SIGNAL(elementAdded(QString,int)),
             this, SLOT(xOnElementAdded(QString,int)));
  disconnect(assembly, SIGNAL(elementRemoved(int)),
             this, SLOT(xOnElementRemoved(int)));
  disconnect(assembly, SIGNAL(elementLowered(int)),
             this, SLOT(xOnElementLowered(int)));
  disconnect(assembly, SIGNAL(elementRaised(int)),
             this, SLOT(xOnElementRaised(int)));
}

bool TreeViewController::xSetLastItemSelected(const QModelIndex& index)
{
  QStandardItem* item = _model->itemFromIndex(index);
  _lastItemSelected.type = static_cast<ItemType>(item->data(TypeRole).toInt());
  if(_lastItemSelected.type == AssemblyType)
  {
    _lastItemSelected.assembly = _link.key(item, NULL);
    _lastItemSelected.isOpen = item->data(OpenRole).toBool();
  }
  else
  {
    _lastItemSelected.assembly = _link.key(item->parent(), NULL);
    _lastItemSelected.isOpen = item->parent()->data(OpenRole).toBool();
  }

  if(_lastItemSelected.type == ElementType &&
     _lastItemSelected.assembly)
  {
    _lastItemSelected.index = item->row() - 1; // -1 because of backgground
  }
  else
    _lastItemSelected.index = -1;
}

} // namespace SvgCompositor
