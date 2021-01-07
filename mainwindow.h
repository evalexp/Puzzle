#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "frame.h"
#include "QFileDialog"
#include "QTimer"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_radioButton_clicked();

    void on_rowSpin_valueChanged(int arg1);

    void on_colSpin_valueChanged(int arg1);

    void on_toolButton_clicked();

    void on_viewOrigin_clicked();

    void on_nextImg_clicked();

    void on_changeImg_clicked();

    void on_resort_clicked();

    void checkGameOver();

    void resetUI();

private:
    Ui::MainWindow *ui;

    Frame *frame;

    int picIndex = 0;
    const int totalPic = 3;
    int limit_time;
    QTimer* timer;

    QString picPath(const int& picIndex);



signals:
    void sendLevel(int);
    void sendSplit(int, int);

public slots:
    void recvSuccess();
};
#endif // MAINWINDOW_H
