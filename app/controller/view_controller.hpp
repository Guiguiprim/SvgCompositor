#ifndef VIEW_CONTROLLER_HPP
#define VIEW_CONTROLLER_HPP

#include <QObject>

#include "misc/move_struct.hpp"

namespace SvgCompose
{
class SvgAssembly;
} // namespace SvgCompose

namespace SvgCompositor
{
class GraphicsItem;
class GraphicsView;

class ViewController : public QObject
{
  Q_OBJECT
public:
  explicit ViewController(GraphicsView* view, QObject *parent = 0);

  void setAssembly(SvgCompose::SvgAssembly* assembly);
  void selectElement(int index);

Q_SIGNALS:
  void selectionChanged(int index);
  void elementsMoved(ItemsMove itemsMove);

private Q_SLOTS:
  void xOnSelectionChanged();

private:
  void xConnectDescription();
  void xCreateItem();

private:
  GraphicsView* _view;
  GraphicsItem* _item;
  SvgCompose::SvgAssembly* _assembly;
};

} // namespace SvgCompositor

#endif // VIEW_CONTROLLER_HPP
