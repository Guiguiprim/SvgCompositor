#include <QApplication>
#include <QStyleFactory>

#include <boost/program_options.hpp>
#include <string>

#include "gui/compositor_widget.hpp"

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("Test");
  QCoreApplication::setApplicationName("SvgUnion");

  po::options_description options("Allowed options");
  options.add_options()
      ("file,f", po::value<std::string>(), "SVG file to clean");
  po::positional_options_description p;
  p.add("file", -1);

  po::variables_map vm;
  try
  {
    po::store(po::command_line_parser(argc, argv).
              options(options).positional(p).run(), vm);
    po::notify(vm);
  }
  catch(const std::exception&) {}

  SvgCompositor::CompositorWidget window;
  if(vm.count("file"))
    window.openProject(vm["file"].as<std::string>().c_str());
  window.show();

  return app.exec();
}
