#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //temporizador para vector command
    timer = new QTimer(this);

    //keepAlive - timer independiente
    keepAlive = new QTimer(this);

    //Conecto y asocio
    connect(timer, SIGNAL(timeout()), this, SLOT(comunicacion()));
    connect(keepAlive, SIGNAL(timeout()), this, SLOT(keepAliveHandler ()));

    //inicio temporizador
    timer->start(TIEMPO/10);
    keepAlive->start(TIEMPO);

    mSocket = new QTcpSocket(this);
    connect(mSocket, &QTcpSocket::readyRead, [&] (){
        QTextStream T(mSocket);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)   //Pregunto si se ha presionado la tecla
{
    //Up
    if(event->key()==Qt::Key_W)
    {
        //Cambio de color
        ui->ButtonUp->setPalette(QPalette(Qt::lightGray));
        //Indico el estado de accion en el vector
        comand[MARCH]=1;
        //mSocket->write("w",1);
    }
    //Down
    if(event->key()==Qt::Key_S)
    {
        ui->ButtonDown->setPalette(QPalette(Qt::lightGray));
        comand[REVER]=1;
        //mSocket->write("s",1);
    }
    //Left
    if(event->key()==Qt::Key_A)
    {
        ui->ButtonL->setPalette(QPalette(Qt::lightGray));
        comand[IZQ]=1;
        //mSocket->write("a",1);
    }
    //Right
    if(event->key()==Qt::Key_D)
    {
        ui->ButtonR->setPalette(QPalette(Qt::lightGray));
        comand[DER]=1;
        //mSocket->write("d",1);
    }
    //Freno
    if(event->key()==Qt::Key_L)
    {
        ui->ButtonBrake->setPalette(QPalette(Qt::lightGray));
        comand[FRENO]=1;
        //mSocket->write("e",1);
    }

    return ;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    //Pregunto si se ha soltado la tecla
    if(event->key()==Qt::Key_W)
    {
        //Regreso el color original
        ui->ButtonUp->setPalette(QPalette());

        //Restablesco el estado
        comand[MARCH]=0;
    }
    if(event->key()==Qt::Key_S)
    {
        ui->ButtonDown->setPalette(QPalette());
        comand[REVER]=0;
    }
    if(event->key()==Qt::Key_A)
    {
        ui->ButtonL->setPalette(QPalette());
        comand[IZQ]=0;
    }
    if(event->key()==Qt::Key_D)
    {
        ui->ButtonR->setPalette(QPalette());
        comand[DER]=0;
    }
    if(event->key()==Qt::Key_L)
    {
        ui->ButtonBrake->setPalette(QPalette());
        comand[FRENO]=0;
    }

    return ;
}


void MainWindow::on_conexion_clicked()
{
    //conexion
    mSocket->connectToHost( ui->textIP->text(), ui->numPUERTO->value() );

    //actualizo estado
    ui->stateText->clear();
    ui->stateText->setText("Conectado");

    //flag de conexion
    conexion = 1;

    logData("CONEXION");
    return ;
}

void MainWindow::cerrarConexion()
{
    mSocket->close();

    //actualizo estado
    ui->stateText->clear();
    ui->stateText->setText("Desconectado");

    //flag de conexion
    conexion = 0;

    logData("DESCONEXION");
}

void MainWindow::on_buttonDisconect_clicked()
{
    cerrarConexion();
}

void MainWindow::on_ButtonSalir_clicked()
{
    QMessageBox box;

    box.setWindowTitle("Exit");
    //Texto principal
    box.setText("¿Desea salir del programa?");
    //Texto informativo
    box.setInformativeText("Perdera control sobre el vehiculo");
    //botones
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);

    if(box.exec()==QMessageBox::Yes)
    {
        this->close();
    }

    //cierro archivo de log
    return;
}

void MainWindow::comunicacion () {

    //detengo el timer
    timer->stop();

    //Se envian las acciones
    sendAccion();

    //continua el timer
    timer->start(TIEMPO/10);
}

void MainWindow::keepAliveHandler () {

    //detengo el timer
    keepAlive->stop();
    timer->stop();

    if (conexion) {

        mSocket->write(TAKE_ALIVE,strlen(TAKE_ALIVE));
    }

    //print por terminal
    //qDebug() << "keepALive :)";

    //continua el timer
    keepAlive->start(TIEMPO);
    timer->start(TIEMPO/10);
}

void MainWindow::logData(QString log)
{
    QFile myFile(logFile);
    QString dataInFileBefore;
    QString dataInFile;

    QString lineToLog;

    lineToLog.append(QDateTime::currentDateTime().date().toString("yyyy:MM:dd"));
    lineToLog.append(" - ");
    lineToLog.append(QDateTime::currentDateTime().toString("hh:mm:ss"));
    lineToLog.append(" - ");
    lineToLog.append("COMMAND: ");
    lineToLog.append(log);
    lineToLog.append('\n');


    if (myFile.open(QFile::ReadOnly)) {
        QTextStream readData (&myFile);
        dataInFileBefore.append(readData.readAll());
        myFile.close();
    }

    if (myFile.open(QFile::WriteOnly)) {
        QTextStream writeData (&myFile);
        writeData << dataInFileBefore;
        writeData << lineToLog;
        myFile.close();
    }

    if (myFile.open(QFile::ReadOnly)) {
        QTextStream readData (&myFile);
        dataInFile.append(readData.readAll());
        myFile.close();
    }

    ui->commandLog->setPlainText(dataInFile);
    ui->commandLog->verticalScrollBar()->setValue(ui->commandLog->verticalScrollBar()->maximum());
}


void MainWindow::sendAccion()
{
    if(comand[MARCH]==1)
    {
        mSocket->write(M,strlen(M));
        logData("Avanzar");
    }
    if(comand[FRENO]==1)
    {
        mSocket->write(F,strlen(F));
        logData("Frenar");
    }
    if(comand[REVER]==1)
    {
        mSocket->write(R,strlen(R));
        logData("Reversa");
    }
    if(comand[DER]==1)
    {
        mSocket->write(D,strlen(D));
        logData("Derecha");
    }
    if(comand[IZQ]==1)
    {
        mSocket->write(I,strlen(I));
        logData("Izquierda");
    }

}

void MainWindow::on_pushButton_clicked()
{
     QFile myFile(logFile);

    if (myFile.open(QFile::WriteOnly)) {
        QTextStream writeData (&myFile);
        writeData << "";
        myFile.close();
    }
    ui->commandLog->clear();
}
