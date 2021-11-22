#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextStream>
#include "composemail.h"

ComposeMail::ComposeMail(QWidget *parent) : QWidget(parent)
{
    QFormLayout *form = new QFormLayout();

    to = new QLineEdit(nullptr);
    subject = new QLineEdit(nullptr);

    emailBody = new QTextEdit(nullptr);
    emailBody->setMinimumSize(350, 500);

    QPushButton *submit = new QPushButton("Send");
    submit->setMaximumSize(70,200);
    connect(submit, &QPushButton::clicked, this, &ComposeMail::handleSubmit);

    msg = new QLabel("");


    QHBoxLayout *btmLayout = new QHBoxLayout();
    btmLayout->addWidget(submit);
    btmLayout->addWidget(msg);


    form->addRow("To* :", to);
    form->addRow("Subject* :", subject);
    form->addRow(emailBody);
    form->addRow(btmLayout);

    setLayout(form);
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

        Sent *sent = new Sent(to->text(), subject->text(), emailBody->document()->toPlainText());

        QJsonDocument document = QJsonDocument::fromJson(file.readAll());
        QJsonObject jsonObj = document.object();
        QJsonArray sentArray = jsonObj["sent"].toArray();
        qDebug() << sentArray;
        QJsonObject sentObj;

        sentObj.insert("to", sent->to_);
        sentObj.insert("subject", sent->subject_);
        sentObj.insert("content", sent->content_);


        sentArray.append(sentObj);
        qDebug() << sentArray;

        jsonObj.insert("sent", sentArray);

        document.setObject(jsonObj);

        QByteArray bytes = document.toJson( QJsonDocument::Indented );
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
