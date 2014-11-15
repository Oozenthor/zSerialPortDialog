#include "zserialdialog.h"

zSerialDialog::zSerialDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::zSerialDialog)
{
  ui->setupUi(this);
  this->setFixedSize(this->size()); // Stop user from changing the dialog size.

  //ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

  // Selected communication port changed event
  connect(ui->serialPortInfoListBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showPortInfo(int)));

  fillPortParameters();
  fillPortInfo();
  loadProfileFromDisk(); // Get saved profile st startup
  ui->applyButton->setFocus();
}

zSerialDialog::~zSerialDialog()
{
  delete ui;
}

zSerialDialog::SerialProfile zSerialDialog::profile() const // Return a valid communication port set of values
{
  return currentProfile;
}

void zSerialDialog::fillPortInfo() // Collect the manufacturer information for each connected serial device.
{
  ui->serialPortInfoListBox->clear();
  static const QString blankString = QObject::tr("N/A");
  QString description;
  QString manufacturer;
  foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
    QStringList list;
    description = info.description();
    manufacturer = info.manufacturer();
    list << info.portName()
         << (!description.isEmpty() ? description : blankString)
         << (!manufacturer.isEmpty() ? manufacturer : blankString)
         << info.systemLocation()
         << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
         << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

    ui->serialPortInfoListBox->addItem(list.first(), list);
  }
}

void zSerialDialog::showPortInfo(int idx) // Show the manufacturer information for each connected serial device.
{
  if (idx != -1) {
    QStringList list = ui->serialPortInfoListBox->itemData(idx).toStringList();
    ui->descriptionLabel->setText(tr("%1").arg(list.at(1)));
    ui->manufacturerLabel->setText(tr("By: %1").arg(list.at(2)));
    ui->locationLabel->setText(tr("Location: %1").arg(list.at(3)));
    ui->vidLabel->setText(tr("Vendor ID: %1").arg(list.at(4)));
    ui->pidLabel->setText(tr("Product ID: %1").arg(list.at(5)));
  }
}

void zSerialDialog::fillPortParameters()  // Fill the list boxes with all possible serial port settings
{
  ui->baudRateBox->addItem(QStringLiteral("1200"), QSerialPort::Baud1200);
  ui->baudRateBox->addItem(QStringLiteral("2400"), QSerialPort::Baud2400);
  ui->baudRateBox->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
  ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
  ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
  ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
  ui->baudRateBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
  ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

  ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
  ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
  ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
  ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);

  ui->parityBox->addItem(QStringLiteral("None"), QSerialPort::NoParity);
  ui->parityBox->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
  ui->parityBox->addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
  ui->parityBox->addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);
  ui->parityBox->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);

  ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
  ui->stopBitsBox->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
#endif
  ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

  ui->flowControlBox->addItem(QStringLiteral("None"), QSerialPort::NoFlowControl);
  ui->flowControlBox->addItem(QStringLiteral("RTS/CTS"), QSerialPort::HardwareControl);
  ui->flowControlBox->addItem(QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl);
}


void zSerialDialog::updateProfile()  // Read all list boxes and set the communication profile based on the contents.
{ // Stringxxxxx entries are for use with text objects to display the profile.
  currentProfile.name = ui->serialPortInfoListBox->currentText();

  currentProfile.baudRate = static_cast<QSerialPort::BaudRate>(ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
  currentProfile.stringBaudRate = QString::number(currentProfile.baudRate);

  currentProfile.dataBits = static_cast<QSerialPort::DataBits>(ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
  currentProfile.stringDataBits = ui->dataBitsBox->currentText();

  currentProfile.parity = static_cast<QSerialPort::Parity>(ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
  currentProfile.stringParity = ui->parityBox->currentText();

  currentProfile.stopBits = static_cast<QSerialPort::StopBits>(ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
  currentProfile.stringStopBits = ui->stopBitsBox->currentText();

  currentProfile.flowControl = static_cast<QSerialPort::FlowControl>(ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
  currentProfile.stringFlowControl = ui->flowControlBox->currentText();
}

void zSerialDialog::saveProfileToDisk()  // Read current list box values and save them to disk
{
  QSettings setting("SerialProfile.ini", QSettings::IniFormat);
  setting.beginGroup("SerialProfile");
  setting.setValue("name", ui->serialPortInfoListBox->currentText());
  setting.setValue("baudRate", ui->baudRateBox->currentIndex());
  setting.setValue("dataBits", ui->dataBitsBox->currentIndex());
  setting.setValue("parity", ui->parityBox->currentIndex());
  setting.setValue("stopBits", ui->stopBitsBox->currentIndex());
  setting.setValue("flowControl", ui->flowControlBox->currentIndex());
  setting.endGroup();
}

void zSerialDialog::loadProfileFromDisk()  // Read saved profile and restore all list box selections
{
  QSettings setting("SerialProfile.ini", QSettings::IniFormat);
  setting.beginGroup("SerialProfile");

  // Look for a matching communication port name
  int index = ui->serialPortInfoListBox->findText(setting.value("name", QString("none")).toString(),Qt::MatchExactly);
  if (index != -1) { // If a matching port name is found set the list box to display the saved communication port
    ui->serialPortInfoListBox->setCurrentIndex(index);
  }
  // Fill the rest of the list boxes based on the current index (fillPortsParameters fills the same every time)
  ui->baudRateBox->setCurrentIndex(setting.value("baudRate", int(3)).toInt());
  ui->dataBitsBox->setCurrentIndex(setting.value("dataBits", int(3)).toInt());
  ui->parityBox->setCurrentIndex(setting.value("parity", int(0)).toInt());
  ui->stopBitsBox->setCurrentIndex(setting.value("stopBits", int(0)).toInt());
  ui->flowControlBox->setCurrentIndex(setting.value("flowControl", int(0)).toInt());
  setting.endGroup();
  updateProfile();
}

void zSerialDialog::on_applyButton_clicked()  // Accept current selection set
{
  updateProfile();
  saveProfileToDisk();
  hide();
}

void zSerialDialog::on_refreshButton_clicked() // Refresh port list (new ports might be added i.e. USB to serial)
{
  fillPortInfo();
}
