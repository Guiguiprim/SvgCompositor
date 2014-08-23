#ifndef COMPOSITION_BUTTON_BAR_HPP
#define COMPOSITION_BUTTON_BAR_HPP

#include <QWidget>

#include "controller/action.hpp"

namespace Ui
{
class ButtonBar;
} // namespace Ui

namespace SvgCompositor
{

class ButtonBar : public QWidget
{
  Q_OBJECT

public:
  explicit ButtonBar(QWidget *parent = 0);
  ~ButtonBar();

  QString name() const;
  qreal size() const;
  qreal scale() const;
  qreal dx() const;
  qreal dy() const;

public slots:
  void setName(const QString& name);
  void setNameValid(bool valid);
  void setSizeValue(int size);
  void setScaleValue(qreal scale);
  void setXPosValue(qreal x);
  void setYPosValue(qreal y);

  void setSelectionExist(bool exist);
  void setSelectionCentrable(bool centrable);
  void setBackgroundExist(bool exist);

  void setButtonsSize(int size);

signals:
  void nameChanged(const QString& name);
  void actionTriggered(Action action);

private slots:
  void xOnActionTriggered(int action);
  void xOnNameEdited();

private:
  Ui::ButtonBar* _ui;
};

} // namespace SvgCompositor

#endif // COMPOSITION_BUTTON_BAR_HPP
