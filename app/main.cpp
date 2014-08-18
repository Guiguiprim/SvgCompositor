#include <QApplication>
#include <QStyleFactory>

#include "mainwindow.hpp"
//#include <svg_compose/svg_compose.hpp>

int main(int argc, char* argv[])
{
#ifdef Q_WS_X11
  QApplication::setStyle(QStyleFactory::create("Cleanlooks"));
#endif

  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("Test");
  QCoreApplication::setApplicationName("SvgUnion");

//  AR::SvgCompose::superposeImages("background/blue.svg", "logo/add.svg", "result/add.svg");
//  AR::SvgCompose::superposeImages("background/blue.svg", "logo/close2.svg", "result/close2.svg");
//  AR::SvgCompose::superposeImages("background/green.svg", "logo/connect2.svg", "result/connect2.svg");
//  AR::SvgCompose::superposeImages("background/blue.svg", "logo/create_folder.svg", "result/create_folder.svg");

//  AR::SvgCompose::superposeImagesCenter("img/background/blue.svg",
//                                      "img/logo/add.svg",
//                                      "img/result_/add.svg");

//  AR::SvgCompose::superposeImagesCenter("img/background/blue.svg",
//                                      "img/logo/close2.svg",
//                                      "img/result_/close2.svg");

//  AR::SvgCompose::superposeImagesCenter("img/background/green.svg",
//                                      "img/logo/connect2.svg",
//                                      "img/result_/connect2.svg");

//  AR::SvgCompose::superposeImagesCenter("img/background/blue.svg",
//                                      "img/logo/create_folder.svg",
//                                      "img/result_/create_folder.svg");

  MainWindow window;
  window.show();

  return app.exec();
}
