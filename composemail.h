#ifndef COMPOSEMAIL_H
#define COMPOSEMAIL_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>

class ComposeMail : public QWidget
{
    Q_OBJECT
    struct Sent {
        QString id_;
        QString subject_;
        QString to_;
        QString from_;
        QString sendDate_;
        QString content_;
        QList<QString> attachments_;
        Sent(QString id, QString subject, QString to, QString from, QString sendDate, QList<QString> attachments, QString content) {
            to_ = to;
            subject_ = subject;
            content_ = content;
            attachments_ = attachments;
            sendDate_ = sendDate;
            from_ = from;
            id_ = id;
        }
    };

public:
    QLineEdit *to;
    QLineEdit *subject;
    QTextEdit *emailBody;
    QLabel *attachments;
    QLabel *msg;

    QList<QString> attachList;

    explicit ComposeMail(QWidget *parent = nullptr);

private slots:
    void handleSubmit();
    void handleAttach();
};

#endif // COMPOSEMAIL_H
