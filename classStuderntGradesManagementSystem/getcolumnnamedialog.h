#ifndef GETCOLUMNNAMEDIALOG_H
#define GETCOLUMNNAMEDIALOG_H

#include <QDialog>

namespace Ui {
class getColumnNameDialog;
}

class getColumnNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getColumnNameDialog(QWidget *parent = nullptr,QString *str_ptr=nullptr);
    ~getColumnNameDialog();
    QString getColumnName();
private slots:
    void on_cancelButton_clicked();

    void on_commitButton_clicked();

private:
    Ui::getColumnNameDialog *ui;
    int ok;
    QString *m_str_ptr;
};

#endif // GETCOLUMNNAMEDIALOG_H
