#ifndef COMPOSITION_GRAPHICS_VIEW_HPP
#define COMPOSITION_GRAPHICS_VIEW_HPP

#include <QGraphicsView>

namespace SvgCompositor
{
class GraphicsItem;

class GraphicsView : public QGraphicsView
{
  Q_OBJECT
public:
  explicit GraphicsView(QWidget *parent = 0);

  void setItem(GraphicsItem* item);
  GraphicsItem* item() const;

signals:

public slots:
  void onComposeSizeChanged();

protected:
  virtual void resizeEvent(QResizeEvent* event);
  virtual void showEvent(QShowEvent* event);
  virtual void mouseMoveEvent(QMouseEvent* event);
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* event);

private:
  void xRefitItem();

private:
  QGraphicsScene* _scene;
  GraphicsItem* _item;
};

} // namespace Composition

#endif // COMPOSITION_GRAPHICS_VIEW_HPP
