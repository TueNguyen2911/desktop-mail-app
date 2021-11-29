#ifndef COMPOSEMAIL_H
#define COMPOSEMAIL_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>

//#include "mailbox.h"
class ComposeMail : public QWidget
{
    Q_OBJECT

    struct MailCP {
        QString id_;
        QString subject_;
        QString to_;
        QString from_;
        QString sendDate_;
        QString content_;
        QList<QString> attachments_;
        MailCP() {

        }
        MailCP(QString id, QString subject, QString to, QString from, QString sendDate, QList<QString> attachments, QString content) {
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

    MailCP mailcp;

    QString draftId;

    //QString id;
    QLineEdit *to;
    QLineEdit *subject;
    QTextEdit *emailBody;
    QLabel *attachments;
    QLabel *msg;

    void writeMails(QString filename, QString prop);

    QList<QString> attachList;

    explicit ComposeMail(QWidget *parent = nullptr);
    void deleteDraft(QString filename = "draft.json", QString prop = "draft");
private slots:
    void handleSubmit();
    void handleAttach();
    void handleClose();
    void closeClickedDraft();
signals:
    void closeClicked();
};

#endif // COMPOSEMAIL_H
