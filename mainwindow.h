#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

#include "types.h"
#include "Widgets/dialog.h"
#include "Widgets/search.h"
#include "Widgets/searchtable.h"
#include "Widgets/autorisation.h"
#include "DataBase/database.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr
                        , Dialog *dialog_window = new Dialog()
                        , Search *search_window = new Search()
                        , SearchTable *searchTable_window = new SearchTable()
                        , Autorisation *autorisation_window = new Autorisation());
    ~MainWindow();

private slots:
    void update_table();

    void add_row();

    void update_row();

    void search_data();

    void action_switchLevel_triggered();

    void action_showLevel_triggered();

    void action_report_triggered();

    void show_autorisation_window();

    void action_print_triggered();

    void switch_user();

    void on_comboBox_tb_currentTextChanged(const QString &arg1) const;

    void on_comboBox_db_currentTextChanged(const QString &arg1) const;

    void on_toolButton_addRow_clicked();

    void on_toolButton_UpdateRow_clicked();

    void on_toolButton_deleteRow_clicked();

    void on_toolButton_searchData_clicked();

    void on_toolButton_viewPhoto_clicked();

private:
    void print_error(const QString &window_tittle, const QString &error, const QString &icon_path);

    void apply_styleSheet();

    Ui::MainWindow *ui_;
    DataBase *db_;
    Dialog* dialog_window_;
    Search* search_window_;
    SearchTable* searchTable_window_;
    Autorisation* autorisation_window_;
    QConnectionType connection_type_;
};
#endif // MAINWINDOW_H
