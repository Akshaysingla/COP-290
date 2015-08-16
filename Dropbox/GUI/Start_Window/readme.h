#ifndef README_H
#define README_H

#include <QDialog>

namespace Ui {
class ReadMe;
}

class ReadMe : public QDialog
{
    Q_OBJECT
    
public:
    explicit ReadMe(QString,QWidget *parent = 0);
    ~ReadMe();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::ReadMe *ui;
};

#endif // README_H
