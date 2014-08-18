#ifndef SVGCLEANER_H
#define SVGCLEANER_H

#include <QDomElement>
#include <QStringList>

class SvgClean
{
public:
  struct RemoveConfig {
    QStringList keep_attr;
    QStringList drop_attr;
    QStringList keep_tag;
    QStringList drop_tag;

    RemoveConfig();
    void addAllowedNamespace(const QString& name);
    void addDropedAttribute(const QString& name);
    void addDropedTag(const QString& name);
  };

public:
  static bool clean(const QString& fileNameIn,
                    const QString& fileNameOut,
                    const RemoveConfig& rc = RemoveConfig());

  static bool clean(const QString& fileNameIn,
                    const RemoveConfig& rc = RemoveConfig());

  static QDomDocument clean(const QDomDocument& xmlIn,
                    const RemoveConfig& rc = RemoveConfig());

  static void cleanElementChildren(QDomElement& parent,
                                    const RemoveConfig& rc = RemoveConfig());
  static void cleanElementAttributes(QDomElement& elem,
                                      const RemoveConfig& rc = RemoveConfig());

  static bool isToRemoveTag(const QString& name,
                              const RemoveConfig& rc = RemoveConfig());
  static bool isToRemoveAttribute(const QString& name,
                              const RemoveConfig& rc = RemoveConfig());

};

#endif // SVGCLEANER_H
