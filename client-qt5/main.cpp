#include "widget.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Screen.me uploader");
    parser.addHelpOption();

    QCommandLineOption areaOption(QStringList() << "a" << "area",
        QCoreApplication::translate("main",
                                    "Take screenshot of area of screen."));
    parser.addOption(areaOption);

    parser.process(a);
    bool area = parser.isSet(areaOption);

    Widget w(0, area);
    w.show();
    w.setWindowState(Qt::WindowFullScreen);
    w.setWindowOpacity(0.5);

    return a.exec();
}
