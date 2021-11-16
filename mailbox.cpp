#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QWidget>
#include "mailbox.h"
#include "mail.h"

MailBox::MailBox(QWidget *parent): QWidget(parent)
{
    mainlayout = new QHBoxLayout();
    inboxlayout = new QVBoxLayout();

    QFile file;
    file.setFileName("mails.json");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QJsonDocument mails = QJsonDocument::fromJson(file.readAll());
    QJsonObject item = mails.object();

    messages = item.value(QString("messages")).toArray();
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
    mail = new MailDetails();
    mainlayout->addLayout(inboxlayout);
    mainlayout->addWidget(mail);
    setLayout(mainlayout);

    file.close();
}

void MailBox::onMailSelect(QString id)
{
    foreach (const QJsonValue & v, messages) {
        QJsonObject obj = v.toObject();
        if(obj.value("id") == id) {
//            qWarning()<<obj;
            this->mail->setDetails(obj);
        }
    }
}

MailBox::~MailBox()
{
}

