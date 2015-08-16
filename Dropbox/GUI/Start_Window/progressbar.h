#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QWidget>

namespace Ui {
class ProgressBar;
}

class ProgressBar : public QWidget
{
    Q_OBJECT
    
public:
    explicit ProgressBar(QWidget *parent = 0);
    ~ProgressBar();
    void setvalue ( int val );
    void close_bar();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::ProgressBar *ui;
};

#endif // PROGRESSBAR_H
