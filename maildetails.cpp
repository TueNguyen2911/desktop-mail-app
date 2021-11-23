#include <QVBoxLayout>
#include <QLabel>
#include <QJsonObject>
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

    fromLabel->setMinimumHeight(10);
    toLabel->setMinimumHeight(10);
    subjectLabel->setMinimumHeight(10);

    mailLayout->addWidget(fromLabel);
    mailLayout->addWidget(toLabel);
    mailLayout->addWidget(subjectLabel);
    mailLayout->addWidget(contentLabel);

    setLayout(mailLayout);
}

void MailDetails::setDetails(QJsonObject mail)
{
    this->mail = new Mail(mail.value("id").toString(), mail.value("subject").toString(), mail.value("to").toString(), mail.value("from").toString(), mail.value("sendDate").toString(), mail.value("receiveDate").toString(), mail.value("content").toString());
    fromLabel->setText("<b>From:</b> " + this->mail->id);
    toLabel->setText("<b>To:</b> " + this->mail->to);
    subjectLabel->setText("<b>Sub:</b> " + this->mail->subject);
    contentLabel->setText(this->mail->content);
}
