#ifndef REPOTSTEMPLATES_H
#define REPOTSTEMPLATES_H

#include <QSqlRelationalTableModel>
#include <QJsonDocument>


class ReportTemplate
{
public:
    ReportTemplate(const QString &template_path="");
    ReportTemplate(const QString &name, bool standart);

    bool isStandart();

    bool setTemplate(const QString &template_path);

    [[nodiscard]] QSqlRelationalTableModel* model(const QSqlDatabase &database) const;
    [[nodiscard]] QString name() const;
    [[nodiscard]] QString top() const;
    [[nodiscard]] QString bottom() const;
    [[nodiscard]] QString signature() const;
private:
    void setName(const QString &name);

    QJsonDocument report_template_;
    bool standart_;
};


QVector<QString> reportsTemplatesPaths(const QString path);


#endif // REPOTSTEMPLATES_H
