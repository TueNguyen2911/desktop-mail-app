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

    composeBox = new ComposeMail();
    connect(composeBox, &ComposeMail::closeClicked, this, &MailBox::onCloseCompose);

    composeBox->setMinimumSize(500,500);
    mainlayout->addWidget(composeBox, 1, 1);

    composeBox->hide();
    mail->show();

    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *inbox = new QPushButton("Inbox");
    QPushButton *sent = new QPushButton("Sent");
    QPushButton *compose = new QPushButton("Compose");
    QPushButton *delButton = new QPushButton("Delete");
    QPushButton *draftBtn = new QPushButton("Drafts");


    topBtns.append(inbox);
    topBtns.append(sent);
    topBtns.append(compose);
    topBtns.append(delButton);
    topBtns.append(draftBtn);

    inbox->setMaximumSize(100,50);
    sent->setMaximumSize(100,50);
    compose->setMaximumSize(100,50);
    delButton->setMaximumSize(100,50);
    draftBtn->setMaximumSize(100,50);

    btns->addWidget(inbox);
    btns->addWidget(sent);
    btns->addWidget(compose);
    btns->addWidget(delButton);
    btns->addWidget(draftBtn);

    connect(inbox, &QPushButton::clicked, this, &MailBox::onInboxClicked);
    connect(sent, &QPushButton::clicked, this, &MailBox::onSentClicked);
    connect(compose, &QPushButton::clicked, this, &MailBox::onComposeClicked);
    connect(delButton, &QPushButton::clicked, this, &MailBox::onDeleteClicked);
    connect(draftBtn, &QPushButton::clicked, this, &MailBox::onDraftClicked);

    mainlayout->addLayout(btns, 0, 0);

    setLayout(mainlayout);
}

void MailBox::onMailSelect(QString id)
{
    qDebug() << id;
    foreach (const QJsonValue & v, messages) {
        QJsonObject obj = v.toObject();
        if(obj.value("id") == id) {
            composeBox->hide();
            mail->show();
            this->selectedMail = id;
            this->mail->setDetails(obj);
        }
    }
}
void MailBox::onDraftSelect(QString id)
{
    for(int i = 0; i < msgLength; i++) {
        m2[i]->blockSignals(true);
    }
    for(int i = 0; i < topBtns.size(); i++) {
        topBtns[i]->blockSignals(true);
    }
    foreach (const QJsonValue & v, messages) {
        QJsonObject obj = v.toObject();

        if(obj.value("id") == id) {
                        qDebug() << obj;
            mail->hide();
            composeBox->draftId = obj.value("id").toString();
            //composeBox->id = obj.value("id").toString();
            composeBox->to->setText(obj.value("to").toString());
            composeBox->subject->setText(obj.value("subject").toString());
            composeBox->emailBody->setText(obj.value("content").toString());
//            composeBox->attachList = obj.value("attachments").toArray();
            //composeBox->msg->setText(obj.value("content").toString());
            //composeBox->id = obj.value("id").toString();
            composeBox->show();

            //mail->show();
            this->selectedDraft = id;


        }
    }
}
void MailBox::onComposeClicked()
{
    mail->hide();
            qDebug() << msgLength;
    for(int i = 0; i < msgLength; i++) {
        m2[i]->blockSignals(true);
    }
    for(int i = 0; i < topBtns.size(); i++) {
        topBtns[i]->blockSignals(true);
    }
    composeBox->draftId = "";
    composeBox->show();

}

void MailBox::readMails(bool draft) {
    QFile file;
    file.setFileName(this->currentInbox);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QJsonDocument mails = QJsonDocument::fromJson(file.readAll());
    QJsonObject item = mails.object();
qDebug() << item;
    wdg = new QWidget;
    QScrollArea *scroll = new QScrollArea;
    QWidget *content_widget = new QWidget;
    inboxlayout = new QVBoxLayout(content_widget);

    QString prop = currentInbox == "mails.json" ? "messages" : "sent";
    if(draft) {
        prop = "draft";
    }
    qDebug() << prop;
    messages = item.value(prop).toArray();
    qDebug() << messages;
    msgLength = messages.size();
    m2 = new Mail*[msgLength + 1];
    int i = 0;
    foreach (const QJsonValue & v, messages) {

        QJsonObject obj = v.toObject();
        QJsonArray attachments = obj["attachments"].toArray();
        QList<QString> atts;
        foreach(const auto & attachment, attachments) {
            atts.push_back(attachment.toString());
        }
        m2[i] = new Mail(obj.value("id").toString(),
                           obj.value("subject").toString(),
                           obj.value("to").toString(),
                           obj.value("from").toString(),
                           obj.value("sendDate").toString(),
                           obj.value("receiveDate").toString(),
                           atts,
                           obj.value("content").toString());
        m2[i]->setMinimumHeight(70);
        m2[i]->setMaximumWidth(250);
        inboxlayout->addWidget(m2[i]);
        m2[i]->blockSignals(false);
        if(!draft) {
            connect(m2[i], &Mail::clicked, this, &MailBox::onMailSelect);
        } else {
            connect(m2[i], &Mail::clicked, this, &MailBox::onDraftSelect);
        }
        i++;
    }

    scroll->setWidget(content_widget);
    wdg->setLayout(new QVBoxLayout);
    wdg->layout()->addWidget(scroll);
    wdg->setMinimumSize(300,500);
    wdg->setMaximumWidth(500);
    mainlayout->addWidget(wdg,1,0);

    setLayout(mainlayout);

    file.close();
}

void MailBox::onDeleteClicked() {
    QString selected = box == "draft" ? selectedDraft : selectedMail;

        for(auto it = messages.begin(); it != messages.end(); it++) {
            if((*it).toObject().value("id")==selected) {
                messages.erase(it);
                writeMails();
                readMails(box == "draft" ? true : false);
                mail->clearDetails();
                break;
            }

    }
}

void MailBox::writeMails() {
    QFile file;
    file.setFileName(this->currentInbox);
    file.open(QIODevice::ReadWrite | QIODevice::Truncate |QIODevice::Text);

    QString prop = currentInbox == "mails.json" ? "messages" : "sent";
    if(currentInbox == "draft.json")
        prop = "draft";
    QJsonObject mails;
    QJsonArray propArr(messages);
    mails.insert(prop, propArr);
    QJsonDocument newDoc(mails);
    file.write(newDoc.toJson());

    file.close();
}

void MailBox::onSentClicked()
{
    box = "sent";
    currentInbox="sent.json";
    mail->clearDetails();
    readMails();
}

void MailBox::onInboxClicked()
{
    box = "inbox";
    currentInbox="mails.json";
    mail->clearDetails();
    readMails();
}
void MailBox::onCloseCompose() {
    composeBox->hide();
    mail->show();
    for(int i = 0; i < msgLength; i++) {
        m2[i]->blockSignals(false);
    }
    for(int i = 0; i < topBtns.size(); i++) {
        topBtns[i]->blockSignals(false);
    }
    onInboxClicked();

}
void MailBox::onDraftClicked() {
    box = "draft";
    currentInbox="draft.json";
    mail->clearDetails();
    readMails(true);
}
MailBox::~MailBox()
{

}
