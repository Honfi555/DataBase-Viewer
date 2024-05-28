#ifndef SEARCHTABLE_H
#define SEARCHTABLE_H

#include <QWidget>
#include <QSqlRelationalTableModel>

namespace Ui {
class SearchTable;
}

class SearchTable : public QWidget
{
    Q_OBJECT

public:
    explicit SearchTable(QWidget *parent = nullptr);
    ~SearchTable();

    void update_table(QSqlRelationalTableModel *search_values);

private slots:
    void on_pushButton_exit_clicked();

private:
    void apple_styleSheet();

    Ui::SearchTable *ui_;
};

#endif // SEARCHTABLE_H
