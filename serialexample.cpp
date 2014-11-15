#include <QDebug>
#include "serialexample.h"

SerialExample::SerialExample(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::SerialExample)
{
  ui->setupUi(this);
  this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);  //Disable resize grip in MS windows
  ui->statusLine->setText(tr("Disconnected"));


  serial = new QSerialPort(this);
  serialDialog = new zSerialDialog;

  connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));

//  timer = new QTimer(this);
//  connect(timer, SIGNAL(timeout()), this, SLOT(signalAdamToReadInputs()));

  //connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
  //connect(serial, SIGNAL(bytesWritten(qint64)), this, SLOT(readData()));
  //connect(serial, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
}

SerialExample::~SerialExample()
{
  disconnectSerialPort();
  delete ui;
}

//****************** Serial Port Functions
void SerialExample::connectSerialPort()
{
  zSerialDialog::SerialProfile profile = serialDialog->profile();  // Get current serial profile

  serial->setPortName(profile.name);
  serial->setBaudRate(profile.baudRate);
  serial->setDataBits(profile.dataBits);
  serial->setParity(profile.parity);
  serial->setStopBits(profile.stopBits);
  serial->setFlowControl(profile.flowControl);
  if (serial->open(QIODevice::ReadWrite)) {
    ui->statusLine->setText(tr("%1 : %2, %3, %4, %5, %6")
                               .arg(profile.name).arg(profile.stringBaudRate).arg(profile.stringDataBits)
                               .arg(profile.stringParity).arg(profile.stringStopBits).arg(profile.stringFlowControl));
//    timer->start(500);
  } else {
    ui->statusLine->setText(tr("Serial error: ") + serial->errorString());
  }
}

void SerialExample::disconnectSerialPort()
{
//  timer->stop();
  serial->close();
  ui->statusLine->setText(tr("Disconnected"));
}

void SerialExample::writeSerialData(QByteArray data)
{
  data.append("/r"); // add newline
  qDebug() << data;
  serial->write(data);
  serial->waitForBytesWritten(1000);
  readData();
}

void SerialExample::signalAdamToReadInputs()
{
  // Adam read inputs command: $016
  writeSerialData("$016\r");
}

void SerialExample::readData()
{
    QByteArray dataIn = serial->readAll();
//    if (dataIn[0] == '!') {
//      // !xx7F00 for door open
//      // !xx7E00 for door closed
//      bool ok;
//      QByteArray baOut = dataIn.mid(2,1);
//      int hexOut = baOut.toInt(&ok, 16);     // hex == 255, ok == true
//      QByteArray baIn = dataIn.mid(4,1);
//      int hexIn = baIn.toInt(&ok, 16);     // hex == 255, ok == true
//      qDebug() << "+" << dataIn << ":" << baOut << baIn;
//      uvOn = static_cast<bool>(bTrue(hexOut, 0));
//      lamp = static_cast<bool>(bTrue(hexOut, 1));
//      door = static_cast<bool>(bTrue(hexIn, 0));
//      QWidget::update();
//    }
    if (dataIn[0] == '>') {
        ui->statusBar->showMessage("Command OK", 2000);
    }
}

void SerialExample::handleError(QSerialPort::SerialPortError error)
{
  if (error == QSerialPort::ResourceError) {
    ui->statusBar->showMessage("Critical Error" + serial->errorString());
    disconnectSerialPort();
  }
}

//****************** Menu
void SerialExample::on_actionExit_triggered()
{
  QApplication::quit();
}

void SerialExample::on_actionConnect_triggered()
{
  connectSerialPort();
}

void SerialExample::on_actionDisconnect_triggered()
{
  disconnectSerialPort();
}

void SerialExample::on_actionConfigure_triggered()
{
  serialDialog->setWindowModality(Qt::ApplicationModal); // Blocks input to all other windows.
  serialDialog->show();
}

//****************** Buttons
void SerialExample::on_command1Button_clicked()
{
    writeSerialData(ui->command1Edit->text().toLocal8Bit());
}

void SerialExample::on_command2Button_clicked()
{
  writeSerialData(ui->command2Edit->text().toLocal8Bit());
}

void SerialExample::on_command3Button_clicked()
{
  writeSerialData(ui->command3Edit->text().toLocal8Bit());
}

void SerialExample::on_command4Button_clicked()
{
  writeSerialData(ui->command4Edit->text().toLocal8Bit());
}


