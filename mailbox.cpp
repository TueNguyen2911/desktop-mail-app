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
#include <QPushButton>
#include "mailbox.h"
#include "mail.h"

MailBox::MailBox(QWidget *parent): QWidget(parent)
{
    mainlayout = new QGridLayout();

    QFile file;
    file.setFileName("mails.json");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QJsonDocument mails = QJsonDocument::fromJson(file.readAll());
    QJsonObject item = mails.object();

    QWidget *wdg = new QWidget;
    QScrollArea *scroll = new QScrollArea;
    QWidget *content_widget = new QWidget;
    inboxlayout = new QVBoxLayout(content_widget);

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

    scroll->setWidget(content_widget);
    wdg->setLayout(new QVBoxLayout);
    wdg->layout()->addWidget(scroll);
    wdg->setMinimumSize(300,500);

    mail = new MailDetails();
    mail->setMinimumSize(500,500);

//<<<<<<< HEAD
//    QPushButton *composeButton = new QPushButton("Compose");
//    QPushButton *starButton = new QPushButton("Star");
//    QPushButton *deleteButton = new QPushButton("Delete");

//    mainlayout->addWidget(composeButton, 0,0);
//    mainlayout->addWidget(starButton, 0,1);
//    mainlayout->addWidget(deleteButton, 0,2);

//    QPushButton *inboxButton = new QPushButton("Inbox");
//    QPushButton *draftsButton = new QPushButton("Drafts");
//    QPushButton *sentButton = new QPushButton("Sent");

//    QGridLayout *inboxTabBar = new QGridLayout();
//    inboxTabBar->addWidget(inboxButton,0,0);
//    inboxTabBar->addWidget(draftsButton,0,1);
//    inboxTabBar->addWidget(sentButton,0,2);

//    QVBoxLayout *sideBar = new QVBoxLayout();
//    sideBar->addLayout(inboxTabBar);
//    sideBar->addWidget(wdg);

//    mainlayout->addLayout(sideBar,1,0);
//=======
    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *compose = new QPushButton("Compose");
    compose->setMaximumSize(100,50);
    QPushButton *sent = new QPushButton("Sent");
    sent->setMaximumSize(100,50);

    btns->addWidget(compose);
    btns->addWidget(sent);

    connect(compose, &QPushButton::clicked, this, &MailBox::onComposeClicked);
    //connect(compose, &QPushButton::clicked, this, &MailBox::onSentClicked);

    mainlayout->addLayout(btns, 0, 0);
    mainlayout->addWidget(wdg,1,0);
//>>>>>>> 54028cc19c893881142cd38f3b27c49aa9c9ba8b
    mainlayout->addWidget(mail,1,1);
    setLayout(mainlayout);

    file.close();
}

void MailBox::onMailSelect(QString id)
{
    foreach (const QJsonValue & v, messages) {
        QJsonObject obj = v.toObject();
        if(obj.value("id") == id) {
            qDebug() << "Clicked";
            this->mail->setDetails(obj);
        }
    }
}
void MailBox::onComposeClicked()
{
    composeBox = new ComposeMail();
    mainlayout->addWidget(composeBox, 1, 1);
}
MailBox::~MailBox()
{
}

