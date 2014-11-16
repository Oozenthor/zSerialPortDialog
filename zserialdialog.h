#ifndef ZSERIALDIALOG_H
#define ZSERIALDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QtAlgorithms>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "ui_zserialdialog.h"

namespace Ui {
class zSerialDialog;
}

class ZSerialDialog : public QDialog
{
  Q_OBJECT

public:
  struct SerialProfile { // Structure to hold a complete set of communication port parameters
    QString name;
    qint32 baudRate;
    QString stringBaudRate;
    QSerialPort::DataBits dataBits;
    QString stringDataBits;
    QSerialPort::Parity parity;
    QString stringParity;
    QSerialPort::StopBits stopBits;
    QString stringStopBits;
    QSerialPort::FlowControl flowControl;
    QString stringFlowControl;
  };

  explicit ZSerialDialog(QWidget *parent = 0);
  ~ZSerialDialog();

  SerialProfile profile() const;

private slots:
  void showPortInfo(int idx);
  void on_applyButton_clicked();

  void on_refreshButton_clicked();

private:
  void loadProfileFromDisk();
  void saveProfileToDisk();
  void fillPortParameters();
  void fillPortInfo();
  void updateProfile();

private:
  Ui::zSerialDialog *ui;  
  SerialProfile currentProfile;
};

#endif // ZSERIALDIALOG_H
