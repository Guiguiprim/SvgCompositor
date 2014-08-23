#include <QApplication>
#include <QStyleFactory>

#include "gui/compositor_widget.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("Test");
  QCoreApplication::setApplicationName("SvgUnion");

  SvgCompositor::CompositorWidget window;
  window.show();

  return app.exec();
}
