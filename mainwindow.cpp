#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListWidget>
#include <QSqlRelationalTableModel>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QMenuBar>
#include <QIcon>

#include "stylesheet.h"
#include "IODataBase/iodatabase.h"
#include "IODataBase/printer.h"
#include "DataBase/connected_level.h"
#include "DataBase/orm.h"
#include "IODataBase/repotstemplates.h"


MainWindow::MainWindow(QWidget *parent
                       , Dialog* dialog_window
                       , Search *search_window
                       , SearchTable *searchTable_window
                       , ReportDialog *report_dialog
                       , Autorisation *autorisation_window)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
    , db_(new ConnectedLevel())
    , dialog_window_(dialog_window)
    , search_window_(search_window)
    , searchTable_window_(searchTable_window)
    , report_dialog_(report_dialog)
    , autorisation_window_(autorisation_window)
    , connection_type_(QConnectionType::Connected)
{
    ui_->setupUi(this);
    this->setWindowTitle("DBViewer");
    this->setWindowIcon(QIcon(":/windowIcons/icons_window.png"));
    
    this->apply_styleSheet();

    ui_->comboBox_db->addItems(db_->get_dataBases());

    connect(dialog_window_, SIGNAL(dialogAddEntered()), this, SLOT(add_row()));
    connect(dialog_window_, SIGNAL(dialogAddEntered()), this, SLOT(update_table()));
    connect(dialog_window_, SIGNAL(dialogUpdateEntered()), this, SLOT(update_row()));
    connect(dialog_window_, SIGNAL(dialogUpdateEntered()), this, SLOT(update_table()));

    connect(search_window_, SIGNAL(searchEntered()), this, SLOT(search_data()));

    connect(autorisation_window_, SIGNAL(credential_entered()), this, SLOT(switch_user()));

    connect(report_dialog_, SIGNAL(reportEntered(ReportTemplate)), this, SLOT(choose_report(ReportTemplate)));

    connect(ui_->actionSwitch_level, SIGNAL(triggered()), this, SLOT(action_switchLevel_triggered()));
    connect(ui_->actionShow_level, SIGNAL(triggered()), this, SLOT(action_showLevel_triggered()));
    connect(ui_->actionReport, SIGNAL(triggered()), this, SLOT(action_report_triggered()));
    connect(ui_->actionSwitch_user, SIGNAL(triggered()), this, SLOT(show_autorisation_window()));
    connect(ui_->actionPrint, SIGNAL(triggered()), this, SLOT(action_print_triggered()));
}


MainWindow::~MainWindow()
{
    delete ui_;
}


/*Updata table with values.*/
void MainWindow::update_table()
{
    this->on_comboBox_tb_currentTextChanged(ui_->comboBox_tb->currentText());
}


/*Calling add row from DB.*/
void MainWindow::add_row()
{
    this->db_->add_row(ui_->comboBox_tb->currentText(), this->dialog_window_->get_dialog_values());
}


/*Calling update row from DB.*/
void MainWindow::update_row()
{
    this->db_->update_row(ui_->comboBox_tb->currentText()
                         , this->dialog_window_->get_dialog_values()
                         , ui_->tableView->selectionModel()->selectedIndexes());
}


/*Calling search data from DB and creating widget with search rezult.*/
void MainWindow::search_data()
{
    QSqlRelationalTableModel *search_query = this->db_->get_searchData(this->search_window_->get_searchCondition(), this->search_window_->get_SearchType());
    this->searchTable_window_->update_table(search_query);
    this->searchTable_window_->show();
}


/*Switch connectiol level Connected/ORM.*/
void MainWindow::action_switchLevel_triggered()
{
    QPair<QString, QString> credential = this->db_->get_credential();

    this->db_->remuve_connection();
    if (QConnectionType::ORM == this->connection_type_) {
        this->db_ = new ConnectedLevel(credential.first, credential.second);
        this->connection_type_ = QConnectionType::Connected;
    }
    else {
        this->db_ = new ORMLevel(credential.first, credential.second);
        this->connection_type_ = QConnectionType::ORM;
    }

    ui_->comboBox_db->clear();
    ui_->comboBox_tb->clear();
    ui_->tableView->setModel(new QSqlRelationalTableModel());

    auto databases = db_->get_dataBases();
    databases.push_front("");
    ui_->comboBox_db->addItems(databases);
}


/*Show connection level in external window.*/
void MainWindow::action_showLevel_triggered()
{
    StyleSheet style_sheet;
    QMessageBox message_box;
    message_box.setWindowTitle("Level");
    message_box.setText("Connection level - " + this->db_->get_level());
    message_box.setStandardButtons(QMessageBox::Ok);
    message_box.setWindowIcon(QIcon(":/settings/icons_showLevel.png"));
    message_box.setStyleSheet(style_sheet.messageBox_styleSheet());
    message_box.exec();
}


/*Create report from tabel.*/
void MainWindow::action_report_triggered()
{
    if (ui_->comboBox_tb->currentText().isEmpty()) {
        this->print_error("User Manual", "Select table to create report.", ":/settings/icons_report.png");
        return;
    }

    IODataBase io_db;
    QString table_name(ui_->comboBox_tb->currentText());

    io_db.write_table(table_name, this->db_->get_allData(table_name), QFileType::csv);
}


/*Show window for enter credential.*/
void MainWindow::show_autorisation_window()
{
    this->autorisation_window_->show();
}


/*Print report.*/
void MainWindow::action_print_triggered()
{
    QVector<ReportTemplate> templates;
    for (const auto &template_path : reportsTemplatesPaths(R"(D:\repos\qt\DB_1\resource\repots templates)")) {
        templates.push_back(ReportTemplate(template_path));
    }

    this->report_dialog_->update_widget(templates);
    this->report_dialog_->show();
}


/*Switch user by entered credential.*/
void MainWindow::switch_user()
{
    QPair<QString, QString> credential = this->autorisation_window_->get_credential();

    this->db_->remuve_connection();
    if (QConnectionType::ORM == this->connection_type_) {
        this->db_ = new ORMLevel(credential.first, credential.second);
        this->connection_type_ = QConnectionType::ORM;
    }
    else {
        this->db_ = new ConnectedLevel(credential.first, credential.second);
        this->connection_type_ = QConnectionType::Connected;
    }

    this->autorisation_window_->clear();
    ui_->comboBox_db->clear();
    ui_->comboBox_tb->clear();
    ui_->tableView->setModel(new QSqlRelationalTableModel());

    auto databases = db_->get_dataBases();
    databases.push_front("");
    ui_->comboBox_db->addItems(databases);
}


/*Set tables from selected DB.*/
void MainWindow::on_comboBox_db_currentTextChanged(const QString &arg1) const
{
    if (arg1.isEmpty()) return;

    this->db_->set_database(arg1);

    ui_->comboBox_tb->clear();
    auto tables = this->db_->get_tables();
    tables.push_front("");
    ui_->comboBox_tb->addItems(tables);
}


/*Set table vales from selected table.*/
void MainWindow::on_comboBox_tb_currentTextChanged(const QString &arg1) const
{
    if (arg1.isEmpty()) return;

    this->ui_->tableView->setModel(this->db_->get_allData(arg1));

    ui_->tableView->resizeColumnsToContents();
}


/*Start process of adding row.*/
void MainWindow::on_toolButton_addRow_clicked()
{
    if (ui_->comboBox_tb->currentText().isEmpty()) {
        this->print_error("User Manual", "Select table to add row.", ":/toolIcons/icon_plus.png");
        return;
    }

    QString table_name = ui_->comboBox_tb->currentText();

    this->dialog_window_->set_dialogType(QDialogType::Add);

    auto column_params = this->db_->get_replacment_columnParams(table_name);
    this->dialog_window_->update_widget(column_params.at(0)
                                        , this->db_->get_dialogData()
                                        , this->db_->get_pri(table_name)
                                        , column_params.at(3)
                                        , ui_->comboBox_tb->currentText()
                                        , this->db_->get_replacment_columnTypes(table_name));
    this->dialog_window_->setModal(true);
    this->dialog_window_->show();
}


/*Start process of updating row in current table.*/
void MainWindow::on_toolButton_UpdateRow_clicked()
{
    if (ui_->comboBox_tb->currentText().isEmpty()) {
        if (ui_->comboBox_tb->currentText().isEmpty()) {
            this->print_error("User Manual", "Select table and row to update data.", ":/toolIcons/icons_edit.png");
            return;
        }
        return;
    }

    QModelIndexList selected = ui_->tableView->selectionModel()->selectedIndexes();

    if (selected.isEmpty()) {
        this->print_error("User Manual", "Select row to update data.", ":/toolIcons/icons_edit.png");
        return;
    }

    QString table_name = ui_->comboBox_tb->currentText();

    this->dialog_window_->set_dialogType(QDialogType::Update);
    this->dialog_window_->update_widget(this->db_->get_replacment_columnParams(table_name).at(0)
                                        , this->db_->get_dialogData()
                                        , this->db_->get_pri(table_name)
                                        , this->db_->get_replacment_columnParams(table_name).at(3)
                                        , table_name
                                        , this->db_->get_replacment_columnTypes(table_name));
    this->dialog_window_->setModal(true);
    this->dialog_window_->show();
}


/*Start process of deleting row form current table.*/
void MainWindow::on_toolButton_deleteRow_clicked()
{
    if (ui_->comboBox_tb->currentText().isEmpty()) {
        this->print_error("User Manual", "Select table and row for deleting.", ":/toolIcons/incons_delete.png");
        return;
    }

    QModelIndexList selected = ui_->tableView->selectionModel()->selectedIndexes();

    if (selected.isEmpty()) {
        this->print_error("User Manual", "Select row for deleting.", ":/toolIcons/incons_delete.png");
        return;
    }

    this->db_->delete_row(ui_->comboBox_tb->currentText(), selected);
    this->update_table();
}


/*Start process of searching data.*/
void MainWindow::on_toolButton_searchData_clicked()
{
    if (ui_->comboBox_tb->currentText().isEmpty()) {
        this->print_error("User Manual", "Select table for searching.", ":/toolIcons/icons_search.png");
        return;
    }

    this->search_window_->set_SearchType(QSearchType::Simple);
    QString table_name = ui_->comboBox_tb->currentText();
    this->search_window_->insert_data(table_name
                                      , this->db_->get_replacment_columnParams(table_name).at(0)
                                      , this->db_->get_replacment_columnTypes(table_name));
    this->search_window_->show();
    this->search_window_->raise();
}


/*Open current photo in other window.*/
void MainWindow::on_toolButton_viewPhoto_clicked()
{
    if (ui_->comboBox_tb->currentText().isEmpty()) {
        this->print_error("User Manual", "Select table and field with path to view photo.", ":/toolIcons/icons_photo.png");
        return;
    }

    QModelIndexList selected = ui_->tableView->selectionModel()->selectedIndexes();

    if (selected.isEmpty()) {
        this->print_error("User Manual", "Select field with image path.", ":/toolIcons/icons_photo.png");
        return;
    }

    StyleSheet style_sheet;
    QMessageBox message_box;
    message_box.setWindowTitle("Photo viewer");
    message_box.setIconPixmap(QPixmap(selected.at(0).data().toString()).scaled(800, 600, Qt::KeepAspectRatio));
    message_box.setStandardButtons(QMessageBox::Ok);
    message_box.setWindowIcon(QIcon(":/toolIcons/icons_photo.png"));
    message_box.setStyleSheet(style_sheet.messageBox_styleSheet());
    message_box.exec();
}


/*Print error via external window.*/
void MainWindow::print_error(const QString &window_tittle, const QString &error, const QString &icon_path)
{
    StyleSheet style_sheet;
    QPixmap icon(icon_path);
    QMessageBox message_box(this);

    message_box.setIconPixmap(icon.scaled(QSize(48, 48)));
    message_box.setWindowTitle(window_tittle);
    message_box.setText(error);
    message_box.setStandardButtons(QMessageBox::Ok);
    message_box.setStyleSheet(style_sheet.messageBox_styleSheet());
    message_box.exec();
}


/*Set css styles to widgets.*/
void MainWindow::apply_styleSheet()
{
    StyleSheet style_sheet;
    this->setStyleSheet(style_sheet.window_styleSheet());

    ui_->menuBar->setStyleSheet(style_sheet.menuBar_styleSheet());
    ui_->menuFile->setStyleSheet(style_sheet.menu_styleSheet());

    ui_->comboBox_db->setStyleSheet(style_sheet.comboBox_styleSheet());
    ui_->comboBox_tb->setStyleSheet(style_sheet.comboBox_styleSheet());

    ui_->tableView->setStyleSheet(style_sheet.table_styleSheet());

    ui_->toolButton_addRow->setStyleSheet(style_sheet.toolButton_styleSheet());
    ui_->toolButton_UpdateRow->setStyleSheet(style_sheet.toolButton_styleSheet());
    ui_->toolButton_deleteRow->setStyleSheet(style_sheet.toolButton_styleSheet());
    ui_->toolButton_searchData->setStyleSheet(style_sheet.toolButton_styleSheet());
    ui_->toolButton_viewPhoto->setStyleSheet(style_sheet.toolButton_styleSheet());
}


void MainWindow::choose_report(ReportTemplate report)
{
    if (!report.isStandart()) {
        print_report(report.name()
                     , report.top()
                     , report.bottom()
                     , report.signature()
                     , report.model(this->db_->get_database()));
        return;
    }

    QSqlRelationalTableModel *model = this->db_->get_allData(ui_->comboBox_tb->currentText());

    print_table(model);
    delete model;
}
