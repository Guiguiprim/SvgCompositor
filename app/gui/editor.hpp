#ifndef COMPOSITION_EDITOR_HPP
#define COMPOSITION_EDITOR_HPP

#include <QWidget>

#include "controller/action.hpp"

class QTextEdit;

namespace SvgCompose
{
class SvgAssembly;
} //


namespace SvgCompositor
{
class ButtonBar;
class ButtonBarController;
class EditorController;
class GraphicsItem;
class GraphicsView;
class ViewController;

class Editor : public QWidget
{
  Q_OBJECT
public:
  explicit Editor(QWidget *parent = 0);
  ~Editor();

  void setAssembly(SvgCompose::SvgAssembly* assembly);
  SvgCompose::SvgAssembly* assembly() const;

private:
  GraphicsView* _view;
  ButtonBar* _buttonBar;
  QTextEdit* _xmlPreview;
  SvgCompose::SvgAssembly* _assembly;

  ViewController* _viewController;
  EditorController* _editorController;
  ButtonBarController* _buttonBarController;

};

} // namespace SvgCompositor

#endif // COMPOSITION_EDITOR_HPP
