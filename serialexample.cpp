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

  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(signalAdamToReadInputs()));

  //connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
  //connect(serial, SIGNAL(bytesWritten(qint64)), this, SLOT(readData()));
  //connect(serial, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
}

SerialExample::~SerialExample()
{
  closeSerialPort();
  delete ui;
}

//****************** Serial Port Functions
void SerialExample::openSerialPort()
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
    timer->start(500);
  } else {
    ui->statusLine->setText(tr("Serial error: ") + serial->errorString());
  }
}

void SerialExample::closeSerialPort()
{
  timer->stop();
  serial->close();
  ui->statusLine->setText(tr("Disconnected"));
}

void SerialExample::AdamCommand(const QByteArray &data)
{
  serial->write(data);
  serial->waitForBytesWritten(1000);
  readData();
}

void SerialExample::signalAdamToReadInputs()
{
  // Adam read inputs command: $016
  AdamCommand("$016\r");
}

void SerialExample::readData()
{
    QByteArray dataIn = serial->readAll();
    if (dataIn[0] == '!') {
      // !xx7F00 for door open
      // !xx7E00 for door closed
      bool ok;
      QByteArray baOut = dataIn.mid(2,1);
      int hexOut = baOut.toInt(&ok, 16);     // hex == 255, ok == true
      QByteArray baIn = dataIn.mid(4,1);
      int hexIn = baIn.toInt(&ok, 16);     // hex == 255, ok == true
//      qDebug() << "+" << dataIn << ":" << baOut << baIn;
//      uvOn = static_cast<bool>(bTrue(hexOut, 0));
//      lamp = static_cast<bool>(bTrue(hexOut, 1));
//      door = static_cast<bool>(bTrue(hexIn, 0));
//      QWidget::update();
    }
    if (dataIn[0] == '>') {
        ui->statusBar->showMessage("Command OK", 2000);
    }
}

void SerialExample::handleError(QSerialPort::SerialPortError error)
{
  if (error == QSerialPort::ResourceError) {
    ui->statusBar->showMessage("Critical Error" + serial->errorString());
    closeSerialPort();
  }
}

//****************** Menu
void SerialExample::on_actionExit_triggered()
{
  QApplication::quit();
}

void SerialExample::on_actionOpenPort_triggered()
{
  openSerialPort();
}

void SerialExample::on_actionClosePort_triggered()
{
  closeSerialPort();
}

void SerialExample::on_actionConfigure_triggered()
{
  serialDialog->setWindowModality(Qt::ApplicationModal); // Blocks input to all other windows.
  serialDialog->show();
}

//****************** Buttons
void SerialExample::on_UVOnButton_clicked()
{
    AdamCommand("#011001\r");
}

void SerialExample::on_UVOffButton_clicked()
{
    AdamCommand("#011000\r");
}

void SerialExample::on_LEDOnButton_clicked()
{
    AdamCommand("#011101\r");
}

void SerialExample::on_LEDOffButton_clicked()
{
  AdamCommand("#011100\r");
}

//****************** Painted
void SerialExample::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);

  QPoint indPos;
  indPos.setX(this->size().width()*.5);
  indPos.setY(this->size().height()*.7);

  QRect indRect = QRect(0,0,78,35);
  indRect.moveCenter(indPos);

//  QPainter painter(this);

//  ZLamp indDoor(indRect.translated(-indRect.width()*1.1, 0), QColor(Qt::green), door);
//  indDoor.setText("Door");
//  indDoor.Render(painter);

//  ZLamp indLamp(indRect, QColor(Qt::yellow), lamp);
//  indLamp.setText("Lamp");
//  indLamp.Render(painter);

//  ZLamp indUV(indRect.translated(indRect.width()*1.1, 0), QColor(Qt::magenta), uvOn);
//  indUV.setText("UV");
//  indUV.Render(painter);
}
