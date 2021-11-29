#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QFileDialog>
#include <QTextStream>
#include <QJsonDocument>
#include<string>
#include "composemail.h"
#include "mailbox.h"
ComposeMail::ComposeMail(QWidget *parent) : QWidget(parent)
{
    QFormLayout *form = new QFormLayout();

    QLabel *from = new QLabel("user@gmail.com");

    to = new QLineEdit(nullptr);
    subject = new QLineEdit(nullptr);
    emailBody = new QTextEdit(nullptr);

    QPushButton *attach = new QPushButton("Attach File");
    attach->setMaximumSize(70,200);
    connect(attach, &QPushButton::clicked, this, &ComposeMail::handleAttach);

    QPushButton *submit = new QPushButton("Send");
    submit->setMaximumSize(70,200);
    connect(submit, &QPushButton::clicked, this, &ComposeMail::handleSubmit);

    QPushButton *close = new QPushButton("Close");
    close->setMaximumSize(70,200);
    close->setStyleSheet("QPushButton {color: red};");
    connect(close, &QPushButton::clicked, this, &ComposeMail::closeClicked);
    connect(close, &QPushButton::clicked, this, &ComposeMail::closeClickedDraft);
    msg = new QLabel("");
    attachments = new QLabel("");
    attachments->setTextFormat(Qt::TextFormat::MarkdownText);

    QHBoxLayout *btmLayout = new QHBoxLayout();
    btmLayout->addWidget(close);
    btmLayout->addWidget(attach);
    btmLayout->addWidget(submit);
    btmLayout->addWidget(msg);


    form->addRow("<b>From:</b>", from);
    form->addRow("<b>To* :</b>", to);
    form->addRow("<b>Subject* :</b>", subject);
    form->addRow(emailBody);
    form->addRow(attachments);
    form->addRow(btmLayout);

    setLayout(form);
}

void ComposeMail::handleAttach()
{
    QFileDialog dialog;
    attachList = dialog.getOpenFileNames(this,
                                         tr("Select File to Attach"), "",
                                         tr("All Files (*)"));
    QString attachmentString = "Attachments: ";
    int length = attachList.length();
    for(int i = 0; i < length; i++) {
        auto attachmentDetail = attachList.at(i).split("/");
        attachmentString += attachmentDetail[attachmentDetail.length() - 1]  + ", ";
    }
    attachments->setText(attachmentString);
}
void ComposeMail::handleClose()
{

}
void ComposeMail::writeMails(QString filename, QString prop) {
    QFile file;
    file.setFileName(filename);
    file.open( QIODevice::ReadOnly | QIODevice::Text );
    QJsonDocument mails = QJsonDocument::fromJson(file.readAll());
    QJsonObject item = mails.object();
    QJsonArray sentArray = item.value(prop).toArray();

    int id = 1000;
    foreach (const QJsonValue & v, sentArray) {
        id = v.toObject().value("id").toInt()+1;
    }
    if(filename == "draft.json" && draftId.length() > 0){
        id = draftId.toInt();
    }
//    if(this->id.length() > 0)
//        id = this->id.toInt();
    MailCP *sent = new MailCP(QString::number(id), subject->text(), to->text(), "user@gmail.com", "02/07/2011 16:47 UTC+3", attachList, emailBody->document()->toPlainText());
    QJsonObject sentObj;
    sentObj.insert("id", sent->id_);
    sentObj.insert("subject", sent->subject_);
    sentObj.insert("to", sent->to_);
    sentObj.insert("from", sent->from_);
    sentObj.insert("sendDate", sent->sendDate_);
    QJsonArray atts;
    foreach(const auto & attachment, attachList) {
        auto attachmentDetail = attachment.split("/");
        atts.push_back(QJsonValue(attachmentDetail[attachmentDetail.length() - 1]));
    }
    sentObj.insert("attachments", atts);
    sentObj.insert("content", sent->content_);

    sentArray.append(sentObj);

    item.insert(prop, sentArray);
    mails.setObject(item);

    QByteArray bytes = mails.toJson( QJsonDocument::Indented );
    file.close();
    qDebug() << mails;
    if( file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
    {
        QTextStream iStream( &file );
        iStream.setEncoding(QStringConverter::Utf8);
        iStream << bytes;
        file.close();
    }
    msg->setStyleSheet("QLabel { color: green; }");
    msg->setText("Email has been sent");
}

void ComposeMail::deleteDraft(QString filename, QString prop) {
    QFile file;
    file.setFileName(filename);
    file.open( QIODevice::ReadOnly | QIODevice::Text );
    QJsonDocument mails = QJsonDocument::fromJson(file.readAll());
    QJsonObject item = mails.object();
    QJsonArray jsonArray = item.value(prop).toArray();
    QString idToDel;

    foreach (const QJsonValue & v, jsonArray) {
            if(idToDel == v.toObject().value("id").toString()) {
                idToDel = v.toObject().value("id").toString();
                break;
            }
    }
    jsonArray.removeAt(idToDel.toInt());
    item.insert(prop, jsonArray);
    mails.setObject(item);
    QByteArray bytes = mails.toJson( QJsonDocument::Indented );
    file.close();
    if( file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
    {
        QTextStream iStream( &file );
        iStream.setEncoding(QStringConverter::Utf8);
        iStream << bytes;
        file.close();
    }
}
void ComposeMail::handleSubmit()
{
    if(to->text().length() == 0 || subject->text().length() == 0 || emailBody->document()->toPlainText().length() == 0) {
        msg->setStyleSheet("QLabel { color: red; }");
        msg->setText("All the fields must be filled");
    }
    else {
        writeMails("sent.json", "sent");
        to->setText("");
        subject->setText("");
        emailBody->setText("");
        deleteDraft();
        draftId = "";

    }
}

void ComposeMail::closeClickedDraft() {
    if(to->text().length() != 0 && subject->text().length() != 0 && emailBody->document()->toPlainText().length() != 0) {
        writeMails("draft.json", "draft");

    }
    to->setText("");
    subject->setText("");
    emailBody->setText("");
    msg->setText("");
}
