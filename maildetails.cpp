#include <QVBoxLayout>
#include <QLabel>
#include <QJsonObject>
#include <QJsonArray>
#include <QPushButton>
#include "maildetails.h"
#include "mail.h"

MailDetails::MailDetails(QWidget *parent) : QWidget(parent)
{
    mailLayout = new QVBoxLayout();
    mailLayout->setAlignment(Qt::AlignTop);

    fromLabel = new QLabel();
    toLabel = new QLabel();
    subjectLabel = new QLabel();
    contentLabel = new QLabel();
    attachmentsLabel = new QLabel();

    fromLabel->setMinimumHeight(10);
    toLabel->setMinimumHeight(10);
    subjectLabel->setMinimumHeight(10);

    mailLayout->addWidget(fromLabel);
    mailLayout->addWidget(toLabel);
    mailLayout->addWidget(subjectLabel);
    mailLayout->addWidget(contentLabel);
    mailLayout->addWidget(attachmentsLabel);

    setLayout(mailLayout);
}

void MailDetails::setDetails(QJsonObject mail)
{
    QJsonArray attachments = mail["attachments"].toArray();
    QList<QString> atts;
    QString attsString = "";
    foreach(const auto & attachment, attachments) {
        atts.push_back(attachment.toString());
        auto attachmentDetail = attachment.toString().split("/");
        attsString += attachmentDetail[attachmentDetail.length() - 1] + "<br>";
    }
    this->mail = new Mail(mail.value("id").toString(), mail.value("subject").toString(),
                          mail.value("to").toString(), mail.value("from").toString(),
                          mail.value("sendDate").toString(), mail.value("receiveDate").toString(),
                          atts, mail.value("content").toString());
    fromLabel->setText("<b>From:</b> " + this->mail->from);
    toLabel->setText("<b>To:</b> " + this->mail->to);
    subjectLabel->setText("<b>Sub:</b> " + this->mail->subject);
    contentLabel->setText(this->mail->content);
    attachmentsLabel->setText(attsString != "" ? ("<br><br><br>Attachments: <br>" + attsString) : "");

}

void MailDetails::clearDetails()
{
    this->mail = new Mail("", "", "", "", "", "", QList<QString>(), "");
    fromLabel->setText("");
    toLabel->setText("");
    subjectLabel->setText("");
    contentLabel->setText("");
}
