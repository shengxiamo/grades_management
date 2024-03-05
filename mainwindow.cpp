#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "ok1";

    qDebug() << "ok2";

    // 通过检测lineEidt变化连接到槽函数实现动态展示的效果
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::on_seekButton_clicked);


    QSqlDatabase::addDatabase("QSQLITE");
    qDebug() << QSqlDatabase::drivers();
    // 初始化数据库连接
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:\\QT\\Projects\\classStuderntGradesManagementSystem\\student&gradesMangement.db");

    if (!db.open()) {
        qDebug() << "无法打开数据库";
    }


    // 创建一个查询模型
    model = new QSqlTableModel(this, db);
    model->setTable("studentsGrades");
    model->select();

    // 将模型与tableView绑定
    ui->tableView->setModel(model);

    // 将表格设置为可编辑模式
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    ui->lineEdit->setEchoMode(QLineEdit::Normal);//设置lineEdit为明文
    ui->lineEdit->setStyleSheet("color: black;");

}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

void MainWindow::on_addRowButton_clicked()
{
    // 插入一行数据
    int rowCount = model->rowCount();
    model->insertRow(rowCount);

    // 获取新插入行的索引
    QModelIndex index = model->index(rowCount, 0);

    // 设置新插入行的数据
    model->setData(index, "");
    model->setData(model->index(rowCount, 1), "");
    model->setData(model->index(rowCount, 2),"");

    // // 提交数据到数据库
    // if (model->submitAll()) {
    //     qDebug() << "New row added successfully!";
    // } else {
    //     qDebug() << "Failed to add new row!";
    // }
}

void MainWindow::on_saveButton_clicked()
{
    // 提交模型的所有更改到数据库
    if (model->submitAll()) {
        qDebug() << "Data saved successfully!";
    } else {
        qDebug() << "Failed to save data!";
    }
}



void MainWindow::on_pushButton_clicked()
{
    // 获取当前选中的行号
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        qDebug() << "No row selected!";
        return;
    }

    // 获取选中行的行号
    int row = selectedIndexes.at(0).row();

    // 删除选中行
    if (model->removeRow(row)) {
        if (model->submitAll()) {
            qDebug() << "Row deleted successfully!";
        } else {
            qDebug() << "Failed to save data after deleting row!";
        }
    } else {
        qDebug() << "Failed to delete row!";
    }
}



void MainWindow::on_seekButton_clicked()
{
    // 获取用户输入的姓名信息
    QString searchText = ui->lineEdit->text();

    // 构造模糊查询的过滤条件
    QString filter = QString("姓名 LIKE '%%1%' OR 学号 LIKE '%%1%'").arg(searchText);

    // 设置过滤条件
    if (!searchText.isEmpty()) {
        model->setFilter(filter);
        model->select();

        // 检查是否有匹配的记录
        if (model->rowCount() == 0) {
            qDebug() << "No matching record found!";
        }
    } else {
        // 如果搜索文本为空，则清除过滤条件，显示所有记录
        model->setFilter("");
        model->select();
    }
}


// void MainWindow::on_lineEdit_textChanged(const QString& text)
// {
//     // 清空下拉框中的内容
//     ui->comboBox->clear();

//     // 如果输入文本为空，则隐藏下拉框
//     if (text.isEmpty()) {
//         ui->comboBox->hide();
//         return;
//     }
// }








