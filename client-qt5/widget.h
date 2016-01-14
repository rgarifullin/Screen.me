#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QRubberBand>
#include <QMouseEvent>
#include <QFile>
#include <QPixmap>
#include <QScreen>
#include <QClipboard>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QHttpPart>
#include <QHttpMultiPart>

class Widget : public QWidget
{
    Q_OBJECT

    bool area;

    QUrl url;
    QFile *image;
    QPoint origin;
    QString filename;
    QString screensFolder;
    QScreen *screen;
    QPixmap originalPixmap;
    QRubberBand *rubberBand;
    QNetworkReply *reply;
    QNetworkAccessManager *manager;
    QClipboard *clipboard;

    QHttpPart *imagePart;
    QHttpMultiPart *multiPart;

    void takeDisplay();
    void newScreenshot();
    void saveScreenshot();
    void cropScreenshot();
    void upload();

public slots:
    void replyFinished();

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

public:
    Widget(QWidget *parent = 0, bool area = false);
    ~Widget();
};

#endif // WIDGET_H
