#ifndef MAILBOX_H
#define MAILBOX_H

#include <QWidget>
#include <QListWidget>
#include <QJsonArray>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QList>
#include <QLabel>

#include "mail.h"
#include "maildetails.h"

class MailBox : public QWidget
{
    Q_OBJECT

private:
    QJsonArray messages;
    MailDetails *mail;
    QGridLayout *mainlayout;
    QVBoxLayout *inboxlayout;

public:
    MailBox(QWidget *parent = nullptr);
    ~MailBox();

private slots:
    void onMailSelect(QString id);
};
#endif // MAILBOX_H
