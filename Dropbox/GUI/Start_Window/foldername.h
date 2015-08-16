#ifndef FOLDERNAME_H
#define FOLDERNAME_H

#include <QDialog>

namespace Ui {
class FolderName;
}

class FolderName : public QDialog
{
    Q_OBJECT
    
public:
    explicit FolderName(QString* ,QWidget *parent = 0);
    ~FolderName();
    
private:
    Ui::FolderName *ui;
};

#endif // FOLDERNAME_H
