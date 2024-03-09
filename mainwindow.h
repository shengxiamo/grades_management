#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "trie.h"

#include <QMainWindow>
#include <QtSql>
#include <QInputDialog>
#include <QCloseEvent>
#include <QCompleter>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void buildTrieTree();
private slots:

    void on_addRowButton_clicked();

    void on_saveButton_clicked();

    void on_pushButton_clicked();

    void on_seekButton_clicked();

    void on_ascendingSortButton_clicked();

    void on_descendingSortButton_clicked();

    void on_lineEdit_textChanged(const QString &text);

    //void on_comboBox_activated(const QString& text);

    void on_addColumnButton_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *model;

    // 将Trie类作为私有属性，只在用户点击保存后再重新构建，平时直接调用即可，大幅节省系统运行时间
    Trie *trie;
    QCompleter *completer;

};
#endif // MAINWINDOW_H
