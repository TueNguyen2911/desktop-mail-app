#ifndef MAIL_H
#define MAIL_H

#include <QObject>
#include <QWidget>

class Mail : public QWidget
{
    Q_OBJECT

public:
    QString id;
    QString subject;
    QString to;
    QString from;
    QString sendDate;
    QString receiveDate;
    QList<QString> attachments;
    QString content;

    Mail(QWidget *parent = nullptr);
    Mail(QString id, QString subject, QString to, QString from, QString sendDate, QString receiveDate, QList<QString> attachments, QString content);
    ~Mail();

protected:
    void mousePressEvent(QMouseEvent *event);

signals:
    void clicked(QString id);
};

#endif // MAIL_H
