#include "stylesheet.h"


/*Return css style for QPushButton.*/
QString StyleSheet::pushButton_styleSheet(int font_size) const
{
    return ("QPushButton {"
           "border: 1px solid " + this->border_color_ + ";"
           "border-radius: 7px;"
           "background-color: " + this->second_color_ + ";"
           "color: " + this->text_color_ + ";"
           "font-size: " + QString::number(font_size) +"pt;"
           "}"

           "QPushButton:hover {"
           "background-color: " + this->hover_color_ + ";"
           "}"

           "QPushButton:pressed {"
           "background-color: " + this->second_color_ + ";"
           "}");
}


/*Return css style for QToolButton.*/
QString StyleSheet::toolButton_styleSheet() const
{
    return ("QToolButton {"
            "border: none;"
            "background-color: transparent;"
            "}");
}


/*Return css style for QComboBox.*/
QString StyleSheet::comboBox_styleSheet(int font_size) const
{
    return ("QComboBox {"
            "background-color: " + this->second_color_ + ";"
            "border: 1px solid " + this->border_color_ + ";"
            "border-radius: 5px;"
            "font-size: " + QString::number(font_size) +"pt;"
            "color: " + this->text_color_ + ";"
            "}"

            "QComboBox QAbstractItemView {"
            "color: " + this->text_color_ + ";"
            "font-size: " + QString::number(font_size) +"pt;"
            "outline: none;"
            "}"

            "QComboBox QAbstractItemView::item:selected {"
            "background-color: " + this->hover_color_ + ";"
            "}"

            "QComboBox QAbstractItemView::item:hover {"
            "background-color: " + this->hover_color_ + ";"
            "border: 2px solid " + this->border_color_ + ";"
            "}"

            "QComboBox QAbstractItemView QScrollBar {"
            "border: none;"
            "width: 10px;"
            "}"

            "QComboBox QAbstractItemView QScrollBar::add-page, QComboBox QAbstractItemView QScrollBar::sub-page  {"
            "background-color: " + this->second_color_ + ";"
            "}"

            "QComboBox QAbstractItemView QScrollBar::add-line, QComboBox QAbstractItemView QScrollBar::sub-line {"
            "background: none;"
            "}");
}


/*Return css style for QMenu.*/
QString StyleSheet::menu_styleSheet(int font_size) const
{
    return ("QMenu {"
            "background-color: " + this->main_color_ + ";"
            "font-size: " + QString::number(font_size) +"pt;"
            "color: " + this->text_color_ + ";"
            "}"

            "QMenu::item:selected {"
            "background-color: " + this->hover_color_ + ";"
            "}");
}


/*Return css style for QMenuBar.*/
QString StyleSheet::menuBar_styleSheet(int font_size) const
{
    return ("QMenuBar {"
            "background-color: " + this->main_color_ + ";"
            "color: " + this->text_color_ + ";"
            "font-size: " + QString::number(font_size) +"pt;"
            "}"

            "QMenuBar::item {"
            "background-color: transparent;"
            "}"

            "QMenuBar::item:focus {"
            "background-color: " + this->hover_color_ + ";"
            "}"
);
}


/*Return css style for QTableView.*/
QString StyleSheet::table_styleSheet(int font_size) const
{
    return ("QTableView {"
            "background-color: " + this->second_color_ + ";"
            "border: 1px solid " + this->border_color_ + ";"
            "border-radius: 5px;"
            "}"

            "QTableView::item {"
            "border: 1px solid " + this->border_color_ + ";"
            "border-radius: 2px;"
            "color: " + this->text_color_ + ";"
            "text-align: center;"
            "}"

            "QTableView::item:selected {"
            "background-color: " + this->main_color_+ ";"
            "}"

            "QHeaderView::section {"
            "background-color: " + this->second_color_ + ";"
            "border: 1px solid " + this->border_color_ + ";"
            "border-radius: 2px;"
            "color: " + this->text_color_ + ";"
            "font-size: " + QString::number(font_size) +"pt;"
            "}");
}


/*Return css style for QLineEdit.*/
QString StyleSheet::lineEdit_styleSheet(int font_size) const
{
    return ("QLineEdit {"
            "color: " + this->text_color_ + ";"
            "font-size: " + QString::number(font_size) +"pt;"
            "background-color: " + this->main_color_ + ";"
            "border: 1px solid " + this->border_color_ + ";"
            "border-radius: 5px;"
            "}"

            "QLineEdit::selection {"
            "color: " + this->text_color_ + ";"
            "background-color: " + this->hover_color_ + ";"
            "}");
}


/*Return css style for QLabel.*/
QString StyleSheet::label_styleSheet(int font_size) const
{
    return ("QLabel {"
            "color: " + this->text_color_ + ";"
            "background-color: transparent;"
            "font-size: " + QString::number(font_size) + "pt;"
            "}");
}


/*Return css style for main window.*/
QString StyleSheet::window_styleSheet() const
{
    return ("background-color: " + this->main_color_ + ";");
}


/*Return css style for QMessageBox.*/
QString StyleSheet::messageBox_styleSheet(int font_size) const
{
    return ("QMessageBox {"
            "background-color: " + this->main_color_ + ";"
            "}"

            "QMessageBox QLabel {"
            "color: " + this->text_color_ + ";"
            "background-color: transparent;"
            "font-size: " + QString::number(font_size) +"pt;"
            "text-align: center;"
            "}"

            "QMessageBox QPushButton {"
            "border: 1px solid " + this->border_color_ + ";"
            "border-radius: 7px;"
            "background-color: " + this->second_color_ + ";"
            "color: " + this->text_color_ + ";"
            "font-size: 11pt;"
            "}"

            "QMessageBox QPushButton:hover {"
            "background-color: " + this->hover_color_ + ";"
            "}"

            "QMessageBox QPushButton:pressed {"
            "background-color: " + this->second_color_ + ";"
            "}");
}

