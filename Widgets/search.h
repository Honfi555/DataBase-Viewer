#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include <QValidator>

#include "types.h"

namespace Ui {
class Search;
}


class Search : public QWidget
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr
                    , const QSearchType &search_type = QSearchType::Simple
                    , const QMap<QString, QString> &columns_types = {});
    ~Search();

    void set_SearchType(const QSearchType &searchType);
    [[nodiscard]] QSearchType get_SearchType() const;

    void insert_data(const QString &table_name
                     , const QVector<QString> &columns_names
                     , const QMap<QString, QString> &columns_types);

    [[nodiscard]] QMap<QString, QString> get_searchCondition() const;

signals:
    void searchEntered();

private slots:
    void on_pushButton_search_clicked();

    void on_pushButton_switchCondition_clicked();

    void on_pushButton_cancel_clicked();

    void on_comboBox_columns_currentTextChanged(const QString &arg1);

private:
    void apply_styleSheet();

    void update_widget(const QString &column_name, const QMap<QString, QString> &columns_types);

    QRegularExpressionValidator* get_validator(const QString &column_type) const;

    void clear_widget();

    QString table_name_;
    QString column_name_;
    Ui::Search *ui_;
    QSearchType search_type_;
    QMap<QString, QString> columns_types_;
};

#endif // SEARCH_H
