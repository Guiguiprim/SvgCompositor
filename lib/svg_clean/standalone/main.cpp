#include <QCoreApplication>

#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <svg_clean/svg_clean.hpp>

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  Q_UNUSED(a)

  po::options_description generic("Generic options");
  generic.add_options()
      ("help,h", "produce help message");

  po::options_description config("Configuration");
  config.add_options()
      ("output,o", po::value< std::string >(), "output file to create (while be overright if already exists)")
      ("keep,k", po::value< std::vector<std::string> >(), "specific xml namespace to keep")
      ("remove-tag,t", po::value< std::vector<std::string> >(), "specific xml tag to remove")
      ("remove-attr,a", po::value< std::vector<std::string> >(), "specific xml attribute to remove");

  po::options_description hidden("Hidden options");
  hidden.add_options()
      ("file,f", po::value< std::string >(), "SVG file to clean");
  po::positional_options_description p;
  p.add("file", -1);

  po::options_description cmdline_options;
  cmdline_options.add(generic).add(config).add(hidden);

  po::options_description visible("Allowed options");
  visible.add(generic).add(config);

  bool help = false;
  po::variables_map vm;
  try
  {
    po::store(po::command_line_parser(argc, argv).
              options(cmdline_options).positional(p).run(), vm);
    po::notify(vm);
  }
  catch(const std::exception&)
  {
    help = true;
  }

  if(vm.count("help"))
    help = true;

  if(vm.count("file") && !help)
  {
    std::string file = vm["file"].as<std::string>();

    SvgClean::RemoveConfig rc;

    if(vm.count("keep"))
    {
      std::vector<std::string> keep = vm["keep"].as< std::vector<std::string> >();
      for(int i=0; i<keep.size(); ++i)
        rc.addAllowedNamespace(keep.at(i).c_str());
    }

    if(vm.count("remove-tag"))
    {
      std::vector<std::string> rtag = vm["remove-tag"].as< std::vector<std::string> >();
      for(int i=0; i<rtag.size(); ++i)
        rc.addDropedTag(rtag.at(i).c_str());
    }

    if(vm.count("remove-attr"))
    {
      std::vector<std::string> rattr = vm["remove-attr"].as< std::vector<std::string> >();
      for(int i=0; i<rattr.size(); ++i)
        rc.addDropedAttribute(rattr.at(i).c_str());
    }

    if(vm.count("output"))
    {
      std::string output = vm["output"].as<std::string>();
      if(output.empty())
        SvgClean::clean(file.c_str(), rc);
      else
        SvgClean::clean(file.c_str(), output.c_str(), rc);
    }
    else
      SvgClean::clean(file.c_str(), rc);
  }
  else
  {
    std::cout << "usage: SvgCleaner [options] file" << std::endl << std::endl;
    std::cout << visible << std::endl;
  }

  return 0;
}
