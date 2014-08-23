#ifndef TREE_VIEW_CONTROLLER_HPP
#define TREE_VIEW_CONTROLLER_HPP

#include <QObject>

namespace SvgCompositor {

class TreeViewController : public QObject
{
  Q_OBJECT
public:
  explicit TreeViewController(QObject *parent = NULL);



};

} // namespace SvgCompositor

#endif // TREE_VIEW_CONTROLLER_HPP
