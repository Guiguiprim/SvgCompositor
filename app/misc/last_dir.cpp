#include "last_dir.hpp"

#include <QSettings>

namespace SvgCompositor
{

QString LastDir::_projectDir = "";

QString LastDir::getProjectDir()
{
  QSettings settings;
  return settings.value("lastProjectDir","").toString();
}

void LastDir::saveProjectDir(const QString& dir)
{
  QSettings settings;
  settings.setValue("lastProjectDir", dir);
}

QString LastDir::getBackgroundDir()
{
  QSettings settings;
  return settings.value("lastBackgroundDir","").toString();
}

void LastDir::saveBackgroundDir(const QString& dir)
{
  QSettings settings;
  settings.setValue("lastBackgroundDir", dir);
}

void LastDir::saveLogoDir(const QString& dir)
{
  QSettings settings;
  settings.setValue("lastLogoDir", dir);
}

QString LastDir::getLogoDir()
{
  QSettings settings;
  return settings.value("lastLogoDir","").toString();
}

} // namespace Composition
