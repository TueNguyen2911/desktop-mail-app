#include "mail.h"

#include <QString>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

Mail::Mail(QWidget *parent): QWidget(parent)
{
    QVBoxLayout *mailLayout = new QVBoxLayout();
    setLayout(mailLayout);
}

Mail::Mail(QString id, QString subject, QString to, QString from, QString sendDate, QString receiveDate, QList<QString> attachments, QString content)
{
    this->id = id;
    this->subject = subject;
    this->to = to;
    this->from = from;
    this->sendDate = sendDate;
    this->receiveDate = receiveDate;
    this->attachments = attachments;
    this->content = content;
    QVBoxLayout *mailLayout = new QVBoxLayout();

    QLabel* fromLabel = new QLabel("<b>" + this->from + "</b>");
    QLabel* subjectLabel = new QLabel(this->subject);
    QLabel* contentLabel = new QLabel(this->content.mid(0,40));

    fromLabel->setMinimumHeight(15);
    subjectLabel->setMinimumHeight(15);
    contentLabel->setMinimumHeight(15);

    mailLayout->addWidget(fromLabel);
    mailLayout->addWidget(subjectLabel);
    mailLayout->addWidget(contentLabel);

    setLayout(mailLayout);

}

void Mail::mousePressEvent(QMouseEvent *event)
{
    emit clicked(this->id);
}

Mail::~Mail()
{

}
