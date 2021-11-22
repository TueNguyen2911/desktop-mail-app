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
        QString to_;
        QString subject_;
        QString content_;
        Sent(QString to, QString subject, QString content) {
            to_ = to;
            subject_ = subject;
            content_ = content;
        }
    };

public:
    QLineEdit *to;
    QLineEdit *subject;
    QTextEdit *emailBody;
    QLabel *msg;

    explicit ComposeMail(QWidget *parent = nullptr);
private slots:
    void handleSubmit();
};

#endif // COMPOSEMAIL_H
