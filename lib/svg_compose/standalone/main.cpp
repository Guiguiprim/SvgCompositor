#include <QCoreApplication>

#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <svg_compose/svg_assemblies_list.hpp>
#include <svg_compose/svg_assembly.hpp>
#include <svg_compose/svg_compose.hpp>

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  Q_UNUSED(a)

  po::options_description generic("Generic options");
  generic.add_options()
      ("help,h", "Produce help message");

  po::options_description config("Configuration");
  config.add_options()
      ("output,o", po::value< std::string >(), "Output folder (over-right output folder from project)")
      ("image,i", po::value< std::vector<std::string> >(), "Specific image to generate (instead of the all project)");

  po::options_description hidden("Hidden options");
  hidden.add_options()
      ("file,f", po::value< std::string >(), "Project file");
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

    SvgCompose::SvgAssembliesList project;

    if(!project.load(file.c_str()))
      return -1;

    QString outputDir;
    if(vm.count("output"))
      outputDir = vm["output"].as<std::string>().c_str();

    if(vm.count("image"))
    {
      QDir pDir = project.dir();
      QDir oDir;
      if(outputDir.isEmpty())
        oDir = project.outputDir();
      else
        oDir = QDir(outputDir);

      oDir.mkdir(oDir.absolutePath());

      SvgCompose::SvgAssembly* assembly = NULL;
      std::vector<std::string> image = vm["image"].as< std::vector<std::string> >();
      for(int i=0; i<image.size(); ++i)
      {
        assembly = project.assembly(image.at(i).c_str());
        if(assembly)
        {
          SvgCompose::generateAssembly(assembly, pDir, oDir);
        }
        else
        {
          std::cout << "error: \"" << image.at(i) << "\" not found in the project" << std::endl;
        }
      }
    }
    else
    {
      SvgCompose::generateAssemblies(&project, outputDir);
    }
  }
  else
  {
    std::cout << "usage: SvgCompose file [options]" << std::endl << std::endl;
    std::cout << visible << std::endl;
  }

  return 0;
}
