#include <QApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>
#include <QDebug>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QSettings>
#include <QDateTime>
#include <QThread>
#include <QDir>

class ImageCapture: public QObject
{
    Q_OBJECT

public:
    inline explicit ImageCapture(QCamera* camera, quint8 cameraId)
        : _imageCapture(new QCameraImageCapture(camera, this))
        , _cameraId(cameraId)
    {
        _imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    }

    inline QCamera* camera() const
    { return qobject_cast<QCamera*>(_imageCapture->mediaObject()); }

    inline quint8 cameraId() const
    { return _cameraId; }

    inline static void setDestinationDirectory(const QString& dir)
    { _destdir = dir; }

    inline static QString destinationDirectory()
    { return _destdir; }

public slots:
    void captureImage(const QByteArray& transactionID)
    {
        QString filename = destinationDirectory() + "/" + transactionID
                + "_" + QString::number(cameraId())
                + "_" + QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
        QCamera* cam = camera();
        cam->searchAndLock();
        _imageCapture->capture(filename.append(".jpg"));
        cam->unlock();
    }

private:
    QCameraImageCapture* _imageCapture;
    quint8 _cameraId;
    static QString _destdir;
};
QString ImageCapture::_destdir;

class ImageCaptureThread: public QThread
{
public:
    inline explicit ImageCaptureThread(ImageCapture* imageCapture, QObject* parent = 0)
        : QThread(parent)
        , _imageCapture(imageCapture) {}

protected:
    void run()
    {
        _imageCapture->camera()->start();
        exec();
    }

    ImageCapture* _imageCapture;
};

class Server : public QTcpServer
{
    Q_OBJECT

public:
    static const int Timeout = 1000;

    explicit Server(const QList<ImageCapture*>& imageCaptures, QObject* parent)
        : QTcpServer(parent)
        , _imageCaptures(imageCaptures)
    {
        connect(this, SIGNAL(newConnection()), SLOT(processConnection()));
        for (ImageCapture* capture: _imageCaptures) {
            connect(this, SIGNAL(newTransaction(QByteArray)),
                    capture, SLOT(captureImage(QByteArray)));
        }
    }

signals:
    void newTransaction(const QByteArray& id);

public slots:
    void processConnection()
    {
        QTcpSocket* const socket = nextPendingConnection();

        forever {
            if (!socket->waitForReadyRead(Timeout)) {
                break;
            }

            const QList<QByteArray> cmd = socket->readAll().split(' ');
            if (cmd.size() == 2 && cmd[0] == "CAPTURE") {
                socket->close();
                socket->deleteLater();
                emit newTransaction(cmd[1]);
                return;
            }

            break;
        }

        socket->abort();
        socket->deleteLater();
    }

public:
    QList<ImageCapture*> _imageCaptures;
};

void addDevice(int deviceId, QList<QByteArray>& devices, QSettings& settings)
{
    const QByteArray device = settings.value("device_"+QByteArray::number(deviceId)).toByteArray();
    if (device.isEmpty()) {
        return;
    }
    devices.append(device);
}

int main(int argc, char** argv)
{
    const int MaxCameraDevice = 4;

    QApplication app(argc, argv);

    QList<QByteArray> cameraDevices;
    QSettings settings("multicap.ini", QSettings::IniFormat);
    settings.beginGroup("Capture");
    for (int i = 0; i < MaxCameraDevice; i++) {
        addDevice(i, cameraDevices, settings);
    }
    ImageCapture::setDestinationDirectory(settings.value("destdir").toString());
    settings.endGroup();

    QList<ImageCapture*> imageCaptureList;
    int cameraId = 0;
    for (const QByteArray& device: cameraDevices) {
        ++cameraId;

        QCameraViewfinder* viewFinder = new QCameraViewfinder(0);

        QCamera* camera = new QCamera(device);
        camera->setViewfinder(viewFinder);
        camera->setCaptureMode(QCamera::CaptureStillImage);

        ImageCapture* imageCapture = new ImageCapture(camera, cameraId);
        imageCaptureList.append(imageCapture);

        ImageCaptureThread* thread = new ImageCaptureThread(imageCapture, &app);
        thread->setObjectName("thread" + QString::number(cameraId));
        imageCapture->moveToThread(thread);
        thread->start();
    }

    Server* server = new Server(imageCaptureList, &app);
    settings.beginGroup("Service");
    if (!server->listen(QHostAddress(settings.value("address").toString()), settings.value("port").toInt())) {
        qCritical() << "Server Error:" << server->serverError() << server->errorString();
        return 1;
    }
    settings.endGroup();

    return app.exec();
}

#include "main.moc"
