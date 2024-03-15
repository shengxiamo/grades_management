#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("成绩管理系统");

    // 通过检测lineEidt变化连接到槽函数实现动态展示的效果
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::on_seekButton_clicked);

    //使用sqlite驱动
    QSqlDatabase::addDatabase("QSQLITE");

    // 初始化数据库连接
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = "..\\classStuderntGradesManagementSystem\\studentandgradesMangement.db";
    db.setDatabaseName(dbPath); // 使用相对路径，保证项目迁移能正常使用

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

    buildTrieTree();
    completer = new QCompleter(this);
    ui->lineEdit->setCompleter(completer);

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
}

void MainWindow::on_saveButton_clicked()
{
    // 提交模型的所有更改到数据库
    if (model->submitAll()) {
        qDebug() << "Data saved successfully!";
    } else {
        qDebug() << "Failed to save data!";
        QMessageBox::warning(this, "错误", "学号已存在，无法添加重复记录！");
    }

    // 调用方法重新构建字典树
     // buildTrieTree();
}

void MainWindow::on_pushButton_clicked()
{
    // 获取当前选中的行和列
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        qDebug() << "No cell selected!";
        return;
    }

    // 收集选中的行和列
    QSet<int> rows;
    QSet<int> columns;
    for (const QModelIndex& index : selectedIndexes) {
        rows.insert(index.row());
        columns.insert(index.column());
    }

    // 判断是否只选择了一行或一列
    if (rows.size() == 1 && columns.size() > 1) {
        // 只选择了一行，删除该行的所有数据
        int row = *rows.constBegin();
        if (model->removeRow(row)) {
            qDebug() << "Row " << row << " deleted successfully!";
        } else {
            qDebug() << "Failed to delete row " << row << "!";
        }
    } else if (columns.size() == 1 && rows.size() > 1) {
        // 只选择了一列，删除该列的所有数据
        int column = *columns.constBegin();

        QString columnName = model->headerData(column, Qt::Horizontal).toString(); // 获取列名
        QString sql = QString("ALTER TABLE studentsGrades DROP COLUMN %1;").arg(columnName);
        QSqlQuery query;
        if (query.exec(sql)) {
            qDebug() << "Column " << columnName << " deleted successfully from database!";
            model->removeColumn(column);
        } else {
            qDebug() << "Failed to delete column " << columnName << " from database!";
        }
    } else {
        // 选择了多行或多列，提示用户选择单行或单列
        qDebug() << "Please select only one row or one column to delete!";
    }

    // 提交删除操作到数据库
    if (model->submitAll()) {
        qDebug() << "Data saved successfully!";
    } else {
        qDebug() << "Failed to save data after deleting row or column!";
    }

    // 重新加载数据
    model->select();
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

void MainWindow::on_ascendingSortButton_clicked()
{
    // 获取用户选择的列号
    int columnIndex = ui->tableView->selectionModel()->currentIndex().column();

    // 获取列名
    QString columnName = model->headerData(columnIndex, Qt::Horizontal).toString();

    // 创建 SQL 查询语句进行升序排序
    QString queryString = QString("SELECT * FROM studentsGrades ORDER BY %1 ASC").arg(columnName);

    // 执行查询
    QSqlQuery query;
    if (query.exec(queryString)) {
        // 清空模型数据

        model->clear();

        // 重新加载模型数据
        model->setQuery(query);
    } else {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    }
}


void MainWindow::on_descendingSortButton_clicked()
{
    // 获取用户选择的列号
    int columnIndex = ui->tableView->selectionModel()->currentIndex().column();

    // 获取列名
    QString columnName = model->headerData(columnIndex, Qt::Horizontal).toString();

    // 创建 SQL 查询语句进行降序排序
    QString queryString = QString("SELECT * FROM studentsGrades ORDER BY %1 DESC").arg(columnName);

    // 执行查询
    QSqlQuery query;
    if (query.exec(queryString)) {
        // 清空模型数据
        model->clear();

        // 重新加载模型数据
        model->setQuery(query);
    } else {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    }
}


void MainWindow::on_lineEdit_textChanged(const QString &text)
{

    // 根据输入前缀搜索匹配的姓名

    QStringList results = trie->search(text);

    completer->setModel(new QStringListModel(results, completer));
}


void MainWindow::on_addColumnButton_clicked()
{
    // 弹出对话框，让用户输入新的科目名称
    QString columnName = QInputDialog::getText(this, tr("增加新列"), tr("输入科目名称"));

    // 如果用户取消了对话框，或者未输入任何名称，则不进行任何操作
    if (columnName.isEmpty())
        return;

    // 在数据库表中添加新的列
    QSqlQuery query;
    QString alterQuery = QString("ALTER TABLE studentsGrades ADD COLUMN %1 TEXT").arg(columnName);
    if (!query.exec(alterQuery)) {
        qDebug() << "Failed to add new column to the table:" << query.lastError().text();
        return;
    }

    // 更新模型以反映数据库的新结构
    model->setQuery("SELECT * FROM studentsGrades"); // 或者使用 setTable("studentsGrades")

    // 确保模型允许编辑
    model->setEditStrategy(QSqlTableModel::OnFieldChange);

    // 在模型中添加新的列
    int columnCount = model->columnCount();
    model->setHeaderData(columnCount - 1, Qt::Horizontal, columnName); // 默认新列是最后一列
}

void MainWindow::buildTrieTree() {

    trie = new Trie(); // 创建新的Trie对象

    int nameColumn = 0; // 姓名列始终为第一列

    // 遍历模型中的每一行，并将姓名插入到 trie 中
    for (int row = 0; row < model->rowCount(); ++row) {
        QModelIndex index = model->index(row, nameColumn);
        QString name = model->data(index).toString();
        trie->insert(name);
    }
}



