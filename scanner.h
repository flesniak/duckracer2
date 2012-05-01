#ifndef SCANNER_H
#define SCANNER_H

#include <QThread>
#include <QByteArray>
#include <QStringList>

static const QStringList baudrates = QStringList() << "50" << "75" << "110" << "134" << "150" << "200" << "300" << "600" << "1200" << "1800" << "2400"
                                                   << "4800" << "9600" << "19.2k" << "38.4k" << "57.6k" << "115.2k" << "230.4k" << "460.8k" << "500k"
                                                   << "576k" << "921.6k" << "1000k" << "1152k" << "1500k" << "2000k" << "2500k" << "3000k"
                                                   << "3500k" << "4000k";

class scanner : public QThread
{
    Q_OBJECT
public:
    explicit scanner(QObject *parent = 0);
    static QByteArray encodeLegacy(unsigned short decoded);
    static unsigned short decodeLegacy(QByteArray encoded);
    static QByteArray encode(unsigned short decoded, unsigned short cypher = 1337);
    static unsigned short decode(QByteArray encoded, unsigned short cypher = 1337);
    static unsigned int baudRateToParameter(int index);
    static unsigned int baudRateToParameter(QString baudRate);
    void setParameters(QString serialPort, QString baudRate);

public slots:
    void stop();

private:
    QString p_serialPort;
    int p_baudRate;
    bool scan;

protected:
    void run();

signals:
    void newData(QByteArray);
};

#endif // SCANNER_H
