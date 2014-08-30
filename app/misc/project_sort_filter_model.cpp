#include "project_sort_filter_model.hpp"

#include "project_model_enum.hpp"

namespace SvgCompositor {

ProjectSortFilterModel::ProjectSortFilterModel(QObject *parent) :
  QSortFilterProxyModel(parent)
{
}

bool ProjectSortFilterModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
  int typeL = left.data(TypeRole).toInt();
  int typeR = right.data(TypeRole).toInt();

  if(typeL == typeR && typeL == AssemblyType)
  {
    QString nameL = left.data(Qt::DisplayRole).toString();
    QString nameR = right.data(Qt::DisplayRole).toString();
    return (QString::compare(nameL, nameR, Qt::CaseInsensitive) < 0) ? true : false;
  }

  return left.row() < right.row();
}

} // namespace SvgCompositor
