#include <QVBoxLayout>
#include <QLabel>
#include <QJsonObject>

#include "maildetails.h"
#include "mail.h"

MailDetails::MailDetails(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mailLayout = new QVBoxLayout();
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
    fromLabel->setText("<b>From:</b> " + mail.value("from").toString());
    toLabel->setText("<b>To:</b> " + mail.value("to").toString());
    subjectLabel->setText("<b>Sub:</b> " + mail.value("subject").toString());
    contentLabel->setText(mail.value("content").toString());
}
