#include "mailbox.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MailBox m;
    m.show();
    return a.exec();
}

