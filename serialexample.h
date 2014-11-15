#ifndef SERIALPORT_H
#define SERIALPORT_H

#define MSG_TIME 1500
#define MAX_CONSOLE 20000

#include <QDebug>
#include <QPlainTextEdit>
#include <QtSerialPort/QSerialPort>
#include "ui_serialexample.h"
#include "zserialdialog.h"

namespace Ui {
class SerialExample;
}
class zSerialDialog;

class SerialExample : public QMainWindow
{
  Q_OBJECT

public:
  explicit SerialExample(QWidget *parent = 0);
  ~SerialExample();

private slots:
  void connectSerialPort();
  void disconnectSerialPort();
  void writeSerialData(QByteArray data);
  void readSerialData();
  void handleError(QSerialPort::SerialPortError error);

  //Menu and tool bar actions
  void on_actionConnect_triggered();
  void on_actionDisconnect_triggered();
  void on_actionConfigure_triggered();
  void on_actionExit_triggered();

  void on_command1Button_clicked();
  void on_command2Button_clicked();
  void on_command3Button_clicked();
  void on_command4Button_clicked();

  void on_actionClear_triggered();

protected:

private:
  Ui::SerialExample *ui;
  zSerialDialog *serialDialog;
  QSerialPort *serialPort;
};

#endif // SERIALPORT_H
