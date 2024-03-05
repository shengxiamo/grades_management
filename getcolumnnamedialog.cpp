#include "getcolumnnamedialog.h"
#include "ui_getcolumnnamedialog.h"

getColumnNameDialog::getColumnNameDialog(QWidget *parent, QString *str_ptr)
    : QDialog(parent)
    , ui(new Ui::getColumnNameDialog)
    , m_str_ptr(str_ptr)
{
    ui->setupUi(this);
}


getColumnNameDialog::~getColumnNameDialog()
{
    delete ui;
}
QString getColumnNameDialog::getColumnName() {
    ui->lineEdit->text();
}

void getColumnNameDialog::on_cancelButton_clicked()
{

    this->close();
}


void getColumnNameDialog::on_commitButton_clicked()
{

}

