#include "widget.h"

#include <QDateTime>
#include <QGuiApplication>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>

Widget::Widget(QWidget *parent, bool area)
    : QWidget(parent)
{
    this->area = area;

    takeDisplay();

    QFile configFile("config.json");
    if (!configFile.open(QIODevice::ReadOnly))
        exit(1);

    QJsonDocument config = QJsonDocument::fromJson(configFile.readAll());

    QJsonObject configObject = config.object();
    screensFolder = configObject["screens_folder"].toString();
    url = configObject["url"].toString();
    if (url.isEmpty())
        exit(1);

    configFile.close();

    if (!area)
        newScreenshot();
}

Widget::~Widget()
{
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    origin = event->pos();

    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    rubberBand->hide();
    newScreenshot();
}

void Widget::newScreenshot()
{
    cropScreenshot();
    saveScreenshot();
    upload();
}

void Widget::takeDisplay()
{
    originalPixmap = QPixmap();
    screen = QGuiApplication::primaryScreen();
    if (screen)
        originalPixmap = screen->grabWindow(0);
}

void Widget::cropScreenshot()
{
    if (area)
    {
        originalPixmap = originalPixmap.copy(
                    rubberBand->geometry().x(),
                    rubberBand->geometry().y(),
                    rubberBand->geometry().width(),
                    rubberBand->geometry().height());
    }
}

void Widget::saveScreenshot()
{
    if (!QDir(screensFolder).exists())
        QDir().mkpath(screensFolder);

    filename = screensFolder +
               QDateTime::currentDateTime().toString("dd-MM-yyyy_HH-mm-ss") +
               ".png";
    originalPixmap.save(filename);
}

void Widget::upload()
{
    image = new QFile(filename, this);
    if (!image->open(QIODevice::ReadOnly))
        exit(1);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader,
                        QVariant("image/png"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("form-data; name=\"image\"; filename=\"" +
                                 filename + "\""));
    imagePart.setBodyDevice(image);

    multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->append(imagePart);

    QNetworkRequest req(url);
    manager = new QNetworkAccessManager(this);
    reply = manager->post(req, multiPart);
    multiPart->setParent(reply);

    QObject::connect(reply, &QNetworkReply::finished, this,
                     &Widget::replyFinished);
}

void Widget::replyFinished()
{
    QString msg("notify-send '");

    if (!reply->error())
    {
        QByteArray bytes = reply->readAll();
        clipboard = QApplication::clipboard();
        clipboard->setText(QString(bytes));
        msg += "Success! You can use link to image from clipboard.'";
    }
    else
    {
        msg += "Error!'";
    }

#ifdef Q_OS_LINUX
        system(msg.toStdString().c_str());
#endif

    image->close();

    reply->deleteLater();
    reply = 0;
    delete reply;

    QApplication::quit();
}
