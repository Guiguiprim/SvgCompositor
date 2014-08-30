#ifndef PROJECT_SORT_FILTER_MODEL_HPP
#define PROJECT_SORT_FILTER_MODEL_HPP

#include <QSortFilterProxyModel>

namespace SvgCompositor
{

class ProjectSortFilterModel : public QSortFilterProxyModel
{
public:
  explicit ProjectSortFilterModel(QObject *parent = NULL);

protected:
  virtual bool lessThan(const QModelIndex& left, const QModelIndex& right) const;
};

} // namespace SvgCompositor

#endif // PROJECT_SORT_FILTER_MODEL_HPP
