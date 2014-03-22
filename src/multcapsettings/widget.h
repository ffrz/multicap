#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>
#include <QStringListModel>
#include <QVector>
#include <QCamera>

class QComboBox;
class QHBoxLayout;
class QCameraViewfinder;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    static const int MaxCamera = 4;

    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void applySettings();
    void reloadSettings();

private:

    Ui::Widget *ui;
    QHBoxLayout* previewsLayout;
    QSettings settings;
    QStringListModel devicesModel;
    QVector<QCamera*> cameras;
    QVector<QComboBox*> comboBoxes;
};

#endif // WIDGET_H
