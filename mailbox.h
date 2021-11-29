#ifndef MAILBOX_H
#define MAILBOX_H

#include <QWidget>
#include <QListWidget>
#include <QJsonArray>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QList>
#include <QLabel>
#include <QPushButton>
#include <QVector>

#include "mail.h"
#include "maildetails.h"
#include "composemail.h"
class MailBox : public QWidget
{
    Q_OBJECT

private:
    QString box;

    QJsonArray messages;
    MailDetails *mail;
    QGridLayout *mainlayout;
    QVBoxLayout *inboxlayout;
    ComposeMail *composeBox;

    QString selectedMail;
    QString selectedDraft;
    QString currentInbox;

    QVector<QPushButton*> topBtns;

    Mail** m2;
    int msgLength;

    QWidget *wdg;

    void readMails(bool draft = false);
    void writeMails();
public:
    MailBox(QWidget *parent = nullptr);
    ~MailBox();

private slots:
    void onMailSelect(QString id);
    void onComposeClicked();
    void onSentClicked();
    void onInboxClicked();
    void onDeleteClicked();
    void onCloseCompose();
    void onDraftClicked();
    void onDraftSelect(QString id);
};
#endif // MAILBOX_H
