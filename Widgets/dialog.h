#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QValidator>


enum QDialogType{
    Add,
    Update
};


namespace Ui {
    class Dialog;
}


class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(const QVector<QString> &columns_names = {}
                    , const QMap<QString, QVector<QString>> &column_values = {}
                    , QWidget *parent = nullptr);
    ~Dialog();

    [[nodiscard]] QVector<QString> get_dialog_values() const;

    void update_widget(const QVector<QString> &columns_names
                       , const QMap<QString, QVector<QString>> &columns_values
                       , const QString &priColumn
                       , const QVector<QString> &autoInc_columns
                       , const QString &table_name
                       , const QMap<QString, QString> &columns_types);

    void set_dialogType(const QDialogType& dialog_type);

private slots:
    void on_pushButton_exit_clicked();

    void on_pushButton_enter_clicked();

    void on_pushButton_browse_clicked();

signals:
    void dialogAddEntered();

    void dialogUpdateEntered();

private:
    void clear_widget();

    void apply_styleSheet();

    QRegularExpressionValidator* get_validator(const QString &column_type) const;

    Ui::Dialog *ui_;

    QVector<QLabel*> labels_;
    QVector<QComboBox*> boxes_;
    QVector<QLineEdit*> lines_;
    QVector<QPushButton*> btns_;

    // {"auto inc", boxes, lines, btns}
    QVector<int> array_numbers_;

    QDialogType dialog_type_;
};

#endif // DIALOG_H
