#include <QCoreApplication>
#include <QTextStream>

#include <svg_clean/svg_clean.hpp>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  QTextStream cout(stdout);
  if(argc < 2)
  {
    cout << "clean_svg fileNameIn [OPTION]" << endl << endl;
    cout << "\t fileNameIn:           The file you want to clean" << endl;
    cout << "\t -o, --output file:    The file to create" << endl;
    cout << "\t -k, --keep:           List of specific namespaces that you want to keep" << endl;
    cout << "\t -t, --remove-tag:     List of specific tags that you want to remove" << endl;
    cout << "\t -a, --remove-attr:    List of specific attributes that you want to remove" << endl << endl;
    cout << "\t Keep only the main namespace, svg and xlink" << endl;
    cout << "\t Remove the metadata balise" << endl;
    cout << "\t Reduce number to only three digits after dot: D*.DDD" << endl << endl;
    return 0;
  }

  SvgClean::RemoveConfig rc;
  int c = 0;
  QString args;
  QString outFile = "";
  for(int i=2; i<argc; ++i)
  {
    args = QString(argv[i]);
    if(args == "-k" || args == "--keep")
      c = 1;
    else if(args == "-t" || args == "--remove-tag")
      c = 2;
    else if(args == "-a" || args == "--remove-attr")
      c = 3;
    else if(args == "-o" || args == "--output")
      c = 4;
    else
    {
      if(c == 1)
        rc.addAllowedNamespace(args);
      else if(c == 2)
        rc.addDropedTag(args);
      else if(c == 3)
        rc.addDropedAttribute(args);
      else if(c == 4)
        outFile = args;
    }
  }

  if(outFile.isEmpty())
    SvgClean::clean(argv[1], rc);
  else
    SvgClean::clean(argv[1], outFile, rc);

  return 0;
}
