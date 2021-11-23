#include <QFile>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>
#include "mailbox.h"
#include "mail.h"

MailBox::MailBox(QWidget *parent): QWidget(parent)
{
    mainlayout = new QGridLayout();

    currentInbox="mails.json";
    readMails();

    mail = new MailDetails();
    mail->setMinimumSize(500,500);
    mainlayout->addWidget(mail,1,1);

    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *inbox = new QPushButton("Inbox");
    QPushButton *sent = new QPushButton("Sent");
    QPushButton *compose = new QPushButton("Compose");
    QPushButton *delButton = new QPushButton("Delete");

    inbox->setMaximumSize(100,50);
    sent->setMaximumSize(100,50);
    compose->setMaximumSize(100,50);

    btns->addWidget(inbox);
    btns->addWidget(sent);
    btns->addWidget(compose);
    btns->addWidget(delButton);

    connect(inbox, &QPushButton::clicked, this, &MailBox::onInboxClicked);
    connect(sent, &QPushButton::clicked, this, &MailBox::onSentClicked);
    connect(compose, &QPushButton::clicked, this, &MailBox::onComposeClicked);
    connect(delButton, &QPushButton::clicked, this, &MailBox::onDeleteClicked);

    mainlayout->addLayout(btns, 0, 0);

    setLayout(mainlayout);
}

void MailBox::onMailSelect(QString id)
{
    foreach (const QJsonValue & v, messages) {
        QJsonObject obj = v.toObject();
        if(obj.value("id") == id) {
            this->selectedMail = id;
            this->mail->setDetails(obj);
        }
    }
}

void MailBox::onComposeClicked()
{
    composeBox = new ComposeMail();
    mainlayout->addWidget(composeBox, 1, 1);
}

void MailBox::readMails() {
    QFile file;
    file.setFileName(this->currentInbox);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QJsonDocument mails = QJsonDocument::fromJson(file.readAll());
    QJsonObject item = mails.object();

    QWidget *wdg = new QWidget;
    QScrollArea *scroll = new QScrollArea;
    QWidget *content_widget = new QWidget;
    inboxlayout = new QVBoxLayout(content_widget);

    QString prop = currentInbox == "mails.json" ? "messages" : "sent";
    messages = item.value(prop).toArray();

    foreach (const QJsonValue & v, messages) {
        QJsonObject obj = v.toObject();
        Mail *m = new Mail(obj.value("id").toString(),
                           obj.value("subject").toString(),
                           obj.value("from").toString(),
                           obj.value("to").toString(),
                           obj.value("sendDate").toString(),
                           obj.value("receiveDate").toString(),
                           obj.value("content").toString());
        m->setMinimumHeight(70);
        m->setMaximumWidth(250);
        inboxlayout->addWidget(m);
        connect(m, &Mail::clicked, this, &MailBox::onMailSelect);
    }

    scroll->setWidget(content_widget);
    wdg->setLayout(new QVBoxLayout);
    wdg->layout()->addWidget(scroll);
    wdg->setMinimumSize(300,500);

    mainlayout->addWidget(wdg,1,0);

    setLayout(mainlayout);

    file.close();
}

void MailBox::onDeleteClicked() {
    for(auto it = messages.begin(); it != messages.end(); it++) {
        if((*it).toObject().value("id")==selectedMail) {
            messages.erase(it);
            writeMails();
            readMails();
            break;
        }
    }
}

void MailBox::writeMails() {
    QFile file;
    file.setFileName(this->currentInbox);
    file.open(QIODevice::ReadWrite | QIODevice::Truncate |QIODevice::Text);

    QString prop = currentInbox == "mails.json" ? "messages" : "sent";
    QJsonObject mails;
    QJsonArray propArr(messages);
    mails.insert(prop, propArr);
    QJsonDocument newDoc(mails);
    file.write(newDoc.toJson());

    file.close();
}

void MailBox::onSentClicked()
{
    currentInbox="sent.json";
    readMails();
}

void MailBox::onInboxClicked()
{
    currentInbox="mails.json";
    readMails();
}

MailBox::~MailBox()
{

}
