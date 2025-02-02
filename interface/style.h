#ifndef CALC_STYLE_H
#define CALC_STYLE_H

#include <QString>
#include <QMap>
#include <QDir>

namespace calc {

    class Style {

        QMap<QString, QString> stylesheetMap;

        QString emptyString;

        Style() {
            QDir dir("styles/");
            QStringList files = dir.entryList(QStringList() << "*.qss", QDir::Files);

            for (const QString &fileName: files) {
                QFile file(dir.filePath(fileName));
                if (file.open(QFile::ReadOnly | QFile::Text)) {
                    QString baseName = QFileInfo(file).baseName();
                    QTextStream ts(&file);
                    QString content = ts.readAll();
                    stylesheetMap[baseName] = content;
                }
            }
            emptyString = "";
        }

    public:

        static QString const &QSSKey(const QString &key) {

            static Style styleInstance;

            if (styleInstance.stylesheetMap.find(key) != styleInstance.stylesheetMap.end())
                return styleInstance.stylesheetMap[key];
            return styleInstance.emptyString;
        }
    };
}

#endif
