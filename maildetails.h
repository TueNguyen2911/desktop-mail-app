#ifndef MAILDETAILS_H
#define MAILDETAILS_H

#include <QWidget>
#include <QLabel>
#include <QJsonObject>
#include <QVBoxLayout>

#include "mail.h"

class MailDetails : public QWidget
{
    Q_OBJECT

    Mail *mail;
    QVBoxLayout *mailLayout;

public:
    explicit MailDetails(QWidget *parent = nullptr);
    void setDetails(QJsonObject mail);
    void clearDetails();

    QLabel* fromLabel;
    QLabel* toLabel;
    QLabel* subjectLabel;
    QLabel* contentLabel;
    QLabel* attachmentsLabel;
};

#endif // MAILDETAILS_H
