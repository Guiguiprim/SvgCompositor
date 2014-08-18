#ifndef BUTTON_BARC_ONTROLLER_HPP
#define BUTTON_BARC_ONTROLLER_HPP

#include <QObject>

#include "action.hpp"

namespace SvgCompose {
class SvgAssembly;
}

namespace SvgCompositor
{
class ButtonBar;

class ButtonBarController : public QObject
{
  Q_OBJECT
public:
  explicit ButtonBarController(ButtonBar* buttonBar, QObject *parent = 0);

  void setAssembly(SvgCompose::SvgAssembly* assembly);

signals:
  void actionTriggered(int index, Action action);
  void nameChanged(const QString& name);

public slots:
  void onSelectionChanged(int index);

private slots:
  void xOnBackgroundChanged(const QString& background);
  void xOnSizeChanged(int size);
  void xOnScaleChanged(int index, qreal scale);
  void xOnDxChanged(int index, qreal dx);
  void xOnDyChanged(int index, qreal dy);
  void xOnPosChanged(int index, qreal dx, qreal dy);

  void xOnActionTriggered(Action action);

private:
  void setUpConnection();
  void tearDownConnection();

private:
  ButtonBar* _buttonBar;
  SvgCompose::SvgAssembly* _assembly;
  int _sIndex;
};

} // namespace Composition

#endif // BUTTON_BARC_ONTROLLER_HPP
