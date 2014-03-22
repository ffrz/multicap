#include "widget.h"
#include "ui_widget.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QHBoxLayout>
#include <QDebug>

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , previewsLayout(0)
    , settings("multicap.ini", QSettings::IniFormat)
{
    ui->setupUi(this);

    comboBoxes.append(ui->device0ComboBox);
    comboBoxes.append(ui->device1ComboBox);
    comboBoxes.append(ui->device2ComboBox);
    comboBoxes.append(ui->device3ComboBox);

    QStringList deviceList;
    deviceList.append("");
    for (const QByteArray device: QCamera::availableDevices()) {
        deviceList.append(device);
    }
    devicesModel.setStringList(deviceList);

    for (int i = 0; i < MaxCamera; i++) {
        cameras.append(0);
        comboBoxes[i]->setModel(&devicesModel);
    }

    previewsLayout = new QHBoxLayout(ui->previewsGroupBox);
    ui->previewsGroupBox->setLayout(previewsLayout);

    for (int i = 0; i < MaxCamera; i++) {
        QCameraViewfinder* finder = new QCameraViewfinder(ui->previewsGroupBox);
        finder->setFixedSize(240, 160);
        previewsLayout->addWidget(finder);
    }

    connect(ui->applyButton, SIGNAL(clicked()), SLOT(applySettings()));
    connect(ui->closeButton, SIGNAL(clicked()), SLOT(close()));
    reloadSettings();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::reloadSettings()
{
    settings.beginGroup("Service");
    ui->serviceAddressEdit->setText(settings.value("address").toString());
    ui->servicePortSpinBox->setValue(settings.value("port").toUInt());
    settings.endGroup();

    settings.beginGroup("Capture");
    ui->destdirEdit->setText(settings.value("destdir").toString());
    ui->device0ComboBox->setCurrentText(settings.value("device_0").toString());
    ui->device1ComboBox->setCurrentText(settings.value("device_1").toString());
    ui->device2ComboBox->setCurrentText(settings.value("device_2").toString());
    ui->device3ComboBox->setCurrentText(settings.value("device_3").toString());
    settings.endGroup();

    for (int i = 0; i < cameras.size(); i++) {
        if (cameras[i]) {
            cameras[i]->stop();
            cameras[i]->deleteLater();
        }


        const QString device = comboBoxes[i]->currentText();
        if (device.isEmpty()) {
            continue;
        }

        cameras[i] = new QCamera(device.toLocal8Bit());
        cameras[i]->setViewfinder(qobject_cast<QCameraViewfinder*>(previewsLayout->itemAt(i)->widget()));
        cameras[i]->start();
    }
}

void Widget::applySettings()
{
    settings.beginGroup("Service");
    settings.setValue("address", ui->serviceAddressEdit->text());
    settings.setValue("port", ui->servicePortSpinBox->value());
    settings.endGroup();

    settings.beginGroup("Capture");
    settings.setValue("destdir", ui->destdirEdit->text());
    settings.setValue("device_0", ui->device0ComboBox->currentText());
    settings.setValue("device_1", ui->device1ComboBox->currentText());
    settings.setValue("device_2", ui->device2ComboBox->currentText());
    settings.setValue("device_3", ui->device3ComboBox->currentText());
    settings.endGroup();

    reloadSettings();
}
