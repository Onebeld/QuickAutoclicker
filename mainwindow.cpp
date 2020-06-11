#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pixmap("://M-Eagle.png");
    pixmap = pixmap.scaled(ui->label_3->size(),Qt::KeepAspectRatio);
    ui->label_3->setPixmap(pixmap);

    RegisterHotKey((HWND)MainWindow::winId(), 100, 0, VK_F8);
    RegisterHotKey((HWND)MainWindow::winId(), 101, 0, VK_F7);

    timer = new QTimer;
    randomTimer = new QTimer;

    connect(timer, SIGNAL(timeout()), this, SLOT(clicking()));
    connect(randomTimer, SIGNAL(timeout()), this, SLOT(randomClicking()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result){
    Q_UNUSED(eventType)
    Q_UNUSED(result)

    MSG *msg = reinterpret_cast<MSG*>(message);
    if (msg->message == WM_HOTKEY){
        if (msg->wParam == 100){
            if (ui->checkBox->isChecked()){
                randomTimer->setInterval(ui->lineEdit->text().toInt());
                POINT p;
                if (GetCursorPos(&p)){
                    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                }
                randomTimer->start();
            }
            else{
                timer->setInterval(ui->lineEdit->text().toInt());
                POINT p;
                if (GetCursorPos(&p)){
                    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                }
                timer->start();
            }
            ui->lineEdit->setEnabled(false);
            ui->checkBox->setEnabled(false);
        }
        if (msg->wParam == 101){
            if (ui->checkBox->isChecked()){
                randomTimer->stop();
            }
            else{
                timer->stop();
            }
            ui->lineEdit->setEnabled(true);
            ui->checkBox->setEnabled(true);
        }
    }

    return false;
}

void MainWindow::clicking(){
    POINT p;
    if (GetCursorPos(&p)){
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }
}

void MainWindow::randomClicking(){
    if (rand() % 80 < 65){
        POINT p;
        if (GetCursorPos(&p)){
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }
    }
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if (ui->lineEdit->text().isEmpty()){
            ui->lineEdit->setText("0");
            ui->lineEdit->setCursorPosition(0);
        }
}
