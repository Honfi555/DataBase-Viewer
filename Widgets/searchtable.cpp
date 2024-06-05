#include "searchtable.h"
#include "ui_searchtable.h"

#include <QSqlRecord>

#include "stylesheet.h"


SearchTable::SearchTable(QWidget *parent)
    : QWidget(parent)
    , ui_(new Ui::SearchTable)
{
    ui_->setupUi(this);
    this->setWindowTitle("Окно результата поиска");
    this->setWindowIcon(QIcon(":/windowIcons/icons_table.png"));

    this->apple_styleSheet();
}


SearchTable::~SearchTable()
{
    delete ui_;
}


/*Update widget.*/
void SearchTable::update_table(QSqlRelationalTableModel *search_values)
{
    this->ui_->tableView_values->clearSelection();
    this->ui_->tableView_values->setModel(search_values);
    this->ui_->tableView_values->resizeColumnsToContents();
}


/*Close widget.*/
void SearchTable::on_pushButton_exit_clicked()
{
    this->close();
}


/*Set css styles to widgets.*/
void SearchTable::apple_styleSheet()
{
    StyleSheet style_sheet;

    this->setStyleSheet(style_sheet.window_styleSheet());

    ui_->label->setStyleSheet(style_sheet.label_styleSheet(20));

    ui_->pushButton_exit->setStyleSheet(style_sheet.pushButton_styleSheet());

    ui_->tableView_values->setStyleSheet(style_sheet.table_styleSheet());
}

