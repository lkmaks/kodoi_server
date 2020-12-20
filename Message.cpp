#include "Message.h"
#include <QString>

Message::Message(const QByteArray &arr)
{
    QString s = QString::fromStdString(arr.toStdString());
    if (s.startsWith("move")) {
        int start = s.indexOf('{');
        int end = s.indexOf('}');
        int mid = s.indexOf(',');
        if (start == -1 || mid == -1 || end == -1 || !(start < mid && mid < end)) {
            mes_type = MessageType::INCORRECT;
            return;
        }
        QStringRef x = s.midRef(start + 1, mid - (start + 1));
        QStringRef y = s.midRef(mid + 1, end - (mid + 1));
        bool ok_x, ok_y;
        int x_coord = x.toInt(&ok_x);
        int y_coord = y.toInt(&ok_y);
        if (!ok_x || !ok_y) {
            mes_type = MessageType::INCORRECT;
            return;
        }
        coords = {x_coord, y_coord};
    }
    else if (s.startsWith("redo")) {

    }
}
