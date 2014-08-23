#ifndef LAST_DIR_HPP
#define LAST_DIR_HPP

#include <QString>

namespace SvgCompositor
{

class LastDir
{
public:
  static QString getProjectDir();
  static void saveProjectDir(const QString& dir);

  static QString getBackgroundDir();
  static void saveBackgroundDir(const QString& dir);

  static QString getLogoDir();
  static void saveLogoDir(const QString& dir);

private:
  static QString _projectDir;
};

} // namespace SvgCompositor

#endif // LAST_DIR_HPP
