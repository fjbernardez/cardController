#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QAbstractButton>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QPalette>
#include <QMessageBox>
#include <QDialog>
#include <QDebug>
#include <iostream>

//header sockets

#include <QWidget>
#include <QTcpSocket>  //QT += network
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

//String
#include <QDialog>
#include <QString>
#include <string.h>

//Uso de timer
#include <QTimer>

//logg
#include <QFile>
#include <QTextStream>
#include <QScrollBar>
#include <QDate>
#include <QDateTime>
#include <string>

//Definiciones para el vector comand
#define IZQ     0
#define DER     1
#define MARCH   2
#define REVER   3
#define FRENO   4

//Definicion para Qtimer
#define TIEMPO  1000  //milisegundos

#define MAX_BUF 100
//Estado de maquina para movimientos (MCU)
#define M		"#w$"
#define F		"#e$"
#define R		"#s$"
#define I		"#a$"
#define D		"#d$"

#define TAKE_ALIVE "#?$"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTcpSocket;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:

    void on_conexion_clicked();

    void on_buttonDisconect_clicked();

    void on_ButtonSalir_clicked();

    void comunicacion();

    void sendAccion();

    void cerrarConexion();

    void keepAliveHandler ();

    void logData(QString log);

private slots:
    void on_pushButton_clicked();

private:

    Ui::MainWindow *ui;

    QTimer *timer;

    QTimer *keepAlive;

    QTcpSocket *mSocket;

    QString logFile = "/home/nafas/Documents/Qt workSpace/cardController/logfile.txt";

    //configuracion
    int comand[5]={0,0,0,0,0};
    //flag de coneccion
    int conexion = 0;

    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);

};

#endif // MAINWINDOW_H
