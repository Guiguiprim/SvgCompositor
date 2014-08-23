#ifndef SVGCOMPOSITOR_PROJECT_WIDGET_HPP
#define SVGCOMPOSITOR_PROJECT_WIDGET_HPP

#include <QWidget>

namespace SvgCompositor
{

class ProjectWidget : public QWidget
{
  Q_OBJECT
public:
  explicit ProjectWidget(QWidget *parent = NULL);

};

} // namespace SvgCompositor

#endif // SVGCOMPOSITOR_PROJECT_WIDGET_HPP
