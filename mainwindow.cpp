#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    frame = new Frame(this->centralWidget());
    frame->setGeometry(10, 10, 600, 550);
    frame->loadImage(this->picPath(picIndex));
    QPixmap pix;
    if(pix.load(this->picPath(picIndex))){
        this->ui->img->setPixmap(pix.scaled(this->ui->img->width(), this->ui->img->height()));
    }

    connect(this, SIGNAL(sendLevel(int)), frame, SLOT(recvLevel(int)));
    connect(frame, SIGNAL(sendSuccess()), this, SLOT(recvSuccess()));

    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkGameOver()));

    ui->timeText->setVisible(false);
    ui->timeText->setStyleSheet("font-size:30px;color:red");
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::on_spinBox_valueChanged
 * @param arg1
 */
void MainWindow::on_spinBox_valueChanged(int arg1)
{
    emit this->sendLevel(arg1);
}

/**
 * @brief MainWindow::picPath
 * @param picIndex
 * @return picPath
 * the path of picture
 */
QString MainWindow::picPath(const int &picIndex){
    return QString(":/img/img/pic%0.jpg").arg(picIndex%totalPic);
}

/**
 * @brief MainWindow::on_viewOrigin_clicked
 */
void MainWindow::on_viewOrigin_clicked()
{
    if(ui->viewOrigin->text() == "查看原图"){
        ui->viewOrigin->setText("隐藏原图");
        ui->img->setVisible(true);
    }else{
        ui->viewOrigin->setText("查看原图");
        ui->img->setVisible(false);
    }
}

/**
 * @brief MainWindow::on_nextImg_clicked
 */
void MainWindow::on_nextImg_clicked()
{
    this->picIndex++;
    QPixmap pix;
    frame->loadImage(this->picPath(picIndex));
    if(pix.load(this->picPath(picIndex))){
        this->ui->img->setPixmap(pix.scaled(this->ui->img->width(), this->ui->img->height()));
    }
}

/**
 * @brief MainWindow::on_changeImg_clicked
 */
void MainWindow::on_changeImg_clicked()
{
    QString imgPath = QFileDialog::getOpenFileName(this, "Open Image", "", "Image File (*.png *.jpg)");
    if(imgPath != ""){
        QPixmap pix;
        if(pix.load(imgPath)){
            this->ui->img->setPixmap(pix.scaled(this->ui->img->width(), this->ui->img->height()));
            frame->loadImage(imgPath);
        }
    }
}

/**
 * @brief MainWindow::on_resort_clicked
 */
void MainWindow::on_resort_clicked()
{
    frame->upsetOrder();
}

/**
 * @brief MainWindow::on_toolButton_clicked
 */
void MainWindow::on_toolButton_clicked()
{
    if(ui->limitTime->value() == 0){
        QMessageBox::information(this, "Tips", "至少设置一个大于0的数！");
        return ;
    }
    ui->limitTime->setVisible(false);
    ui->timeText->setVisible(true);
    ui->timeText->setText(QString("%0").arg(ui->limitTime->value()));
    frame->upsetOrder();
    this->limit_time = ui->limitTime->value();
    ui->picCountText_2->setText("倒计时：");
    timer->start(1000);
}

/**
 * @brief MainWindow::checkGameOver
 */
void MainWindow::checkGameOver(){
    ui->timeText->setText(QString("%0").arg(ui->timeText->text().toInt() - 1));
    if(ui->timeText->text() == "0"){
        qDebug() << "Stop";
        QMessageBox::information(this, "Fail", "挑战失败！");
        timer->stop();
        this->resetUI();
    }
}

/**
 * @brief MainWindow::recvSuccess
 */
void MainWindow::recvSuccess(){
    QMessageBox::information(this, "Congratulations", QString("恭喜你完成了%0!").arg(this->timer->isActive()?"挑战":"拼图"));
    this->timer->stop();
    this->resetUI();
}

/**
 * @brief MainWindow::resetUI
 */
void MainWindow::resetUI(){
    ui->limitTime->setVisible(true);
    ui->timeText->setVisible(false);
    ui->picCountText_2->setText("计时挑战：");
}
