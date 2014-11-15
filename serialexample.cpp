#include <QDebug>
#include <QPlainTextEdit>
#include "serialexample.h"

SerialExample::SerialExample(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::SerialExample)
{
  ui->setupUi(this);
  this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);  //Disable resize grip in MS windows

  ui->replyConsole->setMaximumBlockCount(20000);
  ui->statusLine->setText(tr("Disconnected"));

  serial = new QSerialPort(this);
  serialDialog = new zSerialDialog;

  connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
  connect(serial, SIGNAL(readyRead()), this, SLOT(readSerialData()));
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
  } else {
    ui->statusLine->setText(tr("Serial error: ") + serial->errorString());
  }
}

void SerialExample::disconnectSerialPort()
{
  serial->close();
  ui->statusLine->setText(tr("Disconnected"));
}

void SerialExample::writeSerialData(QByteArray data)
{
  if (serial->isOpen()) { // serial device open
    data.append("/r"); // add newline
    qDebug() << data;
    serial->write(data);
    serial->waitForBytesWritten(1000);
    readSerialData();
  } else {
    ui->statusLine->setText(tr("No serial device open"));
  }
}

void SerialExample::readSerialData()
{
    QByteArray dataIn = serial->readAll();
    ui->replyConsole->insertPlainText(dataIn);
//    if (dataIn[0] == '>') {
//        ui->statusBar->showMessage("Command OK", 2000);
//    }
}

void SerialExample::handleError(QSerialPort::SerialPortError error)
{
  if (error == QSerialPort::ResourceError) {
    ui->statusLine->setText("Critical Error" + serial->errorString());
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
