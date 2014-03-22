#include <QDebug>
#include <QTcpSocket>
#include <QSettings>

#define PRINT_SOCKET_ERROR() qCritical() << "Socket Error" << socket.error() << socket.errorString()

int main(int argc, char** argv)
{
    if (argc != 2) {
        qCritical() << "Usage: multicap [transaction_id]";
        return 1;
    }

    const int Timeout = 1000;

    QSettings settings("multicap.ini", QSettings::IniFormat);
    settings.beginGroup("Service");
    const QString ipAddress = settings.value("address").toString();
    const int port = settings.value("port").toUInt();
    settings.endGroup();

    QTcpSocket socket;
    socket.connectToHost(ipAddress, port);
    if (!socket.waitForConnected(Timeout)) {
        PRINT_SOCKET_ERROR();
        return 1;
    }

    socket.write(QByteArray("CAPTURE " + QByteArray(argv[1])).constData());
    if (!socket.waitForBytesWritten(Timeout)) {
        PRINT_SOCKET_ERROR();
        return 2;
    }

    return  0;
}
