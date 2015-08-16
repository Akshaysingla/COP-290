#ifndef ERROR_WINDOW_H
#define ERROR_WINDOW_H

#include <QWidget>

namespace Ui {
class Error_window;
}

class Error_window : public QWidget
{
    Q_OBJECT
    
public:
    explicit Error_window(char*, QWidget *parent = 0);
    ~Error_window();

private slots:
    void on_pushButton_clicked();
    
private:
    Ui::Error_window *ui;
};

#endif // ERROR_WINDOW_H
