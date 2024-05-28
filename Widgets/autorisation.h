#ifndef AUTORISATION_H
#define AUTORISATION_H

#include <QWidget>
#include <QPair>


namespace Ui {
class Autorisation;
}

class Autorisation : public QWidget
{
    Q_OBJECT

public:
    explicit Autorisation(QWidget *parent = nullptr);
    ~Autorisation();

    [[nodiscard]] QPair<QString, QString> get_credential() const;

    void clear();

signals:
    void credential_entered();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    void apply_styleSheet();

    Ui::Autorisation *ui_;
};

#endif // AUTORISATION_H
