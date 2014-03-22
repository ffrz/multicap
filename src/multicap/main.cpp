#include <QDebug>
#include <QTcpSocket>
#include <QTime>
#include <QTextStream>

#define PRINT_SOCKET_ERROR() qCritical() << "Socket Error" << socket.error() << socket.errorString()

int main(int argc, char** argv)
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    QTime performanceTime = QTime::currentTime();
    performanceTime.start();

    const int Timeout = 5 * 1000;

    const QString ipAddress = "127.0.0.1";
    const int port = 4321;

    QTcpSocket socket;
    socket.connectToHost(ipAddress, port);

    if (!socket.waitForConnected(Timeout)) {
        PRINT_SOCKET_ERROR();
        return 1;
    }


    socket.write("CAPTURE 1");
    if (!socket.waitForBytesWritten(Timeout)) {
        PRINT_SOCKET_ERROR();
        return 2;
    }

    qDebug() << "complete in" << performanceTime.elapsed() << "ms";

    return  0;
}
