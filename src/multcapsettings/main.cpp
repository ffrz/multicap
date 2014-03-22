#include "widget.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("Multicap Settings");
    app.setApplicationDisplayName(app.applicationName());
    app.setApplicationVersion("1.0.0");

    Widget widget;
    widget.showMaximized();

    return app.exec();
}
