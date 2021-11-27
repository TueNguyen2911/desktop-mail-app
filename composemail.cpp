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

    msg = new QLabel("");
    attachments = new QLabel("");
    attachments->setTextFormat(Qt::TextFormat::MarkdownText);

    QHBoxLayout *btmLayout = new QHBoxLayout();
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

void ComposeMail::handleSubmit()
{
    if(to->text().length() == 0 || subject->text().length() == 0 || emailBody->document()->toPlainText().length() == 0) {
        qDebug() << "Empty";
        msg->setStyleSheet("QLabel { color: red; }");
        msg->setText("All the fields must be filled");
    }
    else {
        QFile file;
        file.setFileName("sent.json");
        file.open( QIODevice::ReadOnly | QIODevice::Text );
        QJsonDocument mails = QJsonDocument::fromJson(file.readAll());
        QJsonObject item = mails.object();
        QJsonArray sentArray = item.value("sent").toArray();

        int id = 1000;
        foreach (const QJsonValue & v, sentArray) {
            id++;
        }
        Sent *sent = new Sent(QString::number(id), subject->text(), to->text(), "user@gmail.com", "02/07/2011 16:47 UTC+3", attachList, emailBody->document()->toPlainText());
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

        item.insert("sent", sentArray);
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
        msg->setStyleSheet("QLabel { color: green; }");
        msg->setText("Email has been sent");
    }
}
