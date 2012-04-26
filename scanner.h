#ifndef SCANNER_H
#define SCANNER_H

#include <QThread>
#include <QByteArray>

class scanner : public QThread
{
    Q_OBJECT
public:
    explicit scanner(QObject *parent = 0);
    static QByteArray encodeLegacy(unsigned short decoded);
    static unsigned short decodeLegacy(QByteArray encoded);
    static QByteArray encode(unsigned short decoded, unsigned short cypher = 0x1337);
    static unsigned short decode(QByteArray encoded, unsigned short cypher = 0x1337);
};

#endif // SCANNER_H
