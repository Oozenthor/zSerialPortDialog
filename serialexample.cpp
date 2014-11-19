#include "serialexample.h"

//****************** Main Window and constructors ******************
SerialExample::SerialExample(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::SerialExample)
{
  ui->setupUi(this);
  this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);  //Disable resize grip in MS windows

  ui->replyConsole->setMaximumBlockCount(MAX_CONSOLE);
//  ui->replyConsole->document()->size().height();
  ui->statusLine->setText("Disconnected");

  serialPort = new QSerialPort(this);

  serialDialog = new ZSerialDialog;
  serialDialog->setWindowModality(Qt::ApplicationModal); // Blocks input to all other windows.

  connect(serialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
  connect(serialPort, SIGNAL(readyRead()), this, SLOT(readSerialData()));
}

SerialExample::~SerialExample()
{
  disconnectSerialPort();
  delete ui;
}

//****************** Serial Port Functions ******************
void SerialExample::connectSerialPort()
{
  if (serialPort->isOpen()) { // serial device open
    ui->statusBar->showMessage("Serial device already connected!", MSG_TIME);
  } else {
    ZSerialDialog::SerialProfile profile = serialDialog->profile();  // Get current serial profile

    serialPort->setPortName(profile.name);
    serialPort->setBaudRate(profile.baudRate);
    serialPort->setDataBits(profile.dataBits);
    serialPort->setParity(profile.parity);
    serialPort->setStopBits(profile.stopBits);
    serialPort->setFlowControl(profile.flowControl);
    if (serialPort->open(QIODevice::ReadWrite)) {
      ui->statusLine->setText(QString("%1 : %2, %3, %4, %5, %6")
                              .arg(profile.name).arg(profile.stringBaudRate).arg(profile.stringDataBits)
                              .arg(profile.stringParity).arg(profile.stringStopBits).arg(profile.stringFlowControl));
      ui->statusBar->showMessage("Serial device connected.", MSG_TIME);
    } else {
      ui->statusLine->setText(tr("Serial error: ") + serialPort->errorString());
    }
  }
}

void SerialExample::disconnectSerialPort()
{
  if (serialPort->isOpen()) { // serial device open
    serialPort->close();
    ui->statusLine->setText("Disconnected");
    ui->statusBar->showMessage("Serial device disconnected.", MSG_TIME);
  } else {
    ui->statusBar->showMessage("Serial device already disconnected!", MSG_TIME);
  }
}

void SerialExample::writeSerialData(QByteArray data)
{
  if (serialPort->isOpen()) { // serial device open
    ui->statusBar->showMessage("Sending: " + data, MSG_TIME);
    data.append("\r"); // add newline
    qDebug() << data;
    serialPort->write(data);
    serialPort->waitForBytesWritten(1000); // Provide serial device blocking.
//    readSerialData();
  } else {
    ui->statusLine->setText("No serial device open");
    ui->statusBar->showMessage("Transmission failed.", MSG_TIME);
  }
}

void SerialExample::readSerialData()
{
  ui->replyConsole->insertPlainText(serialPort->readAll());

  QTextCursor c = ui->replyConsole->textCursor();
  c.movePosition(QTextCursor::End);
  ui->replyConsole->setTextCursor(c);

//  ui->replyConsole->ensureCursorVisible();

}

void SerialExample::handleError(QSerialPort::SerialPortError error)
{
  if (error == QSerialPort::ResourceError) {
    ui->statusLine->setText("Critical Error" + serialPort->errorString());
    disconnectSerialPort();
  }
}

//****************** Menu & Toolbar ******************
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
  serialDialog->show();
}

void SerialExample::on_actionClear_triggered()
{
  ui->replyConsole->clear();
}

//****************** Buttons ******************
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
