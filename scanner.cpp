#include "scanner.h"

scanner::scanner(QObject *parent) : QThread(parent)
{
}

/*-------------------------------------------+
| About duckracer encryption algorithms: The |
| new method eats a 16bit-number, while the  |
| 16th bit must be 0, and outputs an array   |
| of three printable ASCII characters        |
| suitable for code39 barcodes.              |
| The old, legacy algorithm is just provided |
| for reference, i.e. scanning old labels.   |
\-------------------------------------------*/

QByteArray scanner::encode(unsigned short decoded, unsigned short cypher)
{
    if( decoded >> 15 )
        return QByteArray();
    cypher &= 32767; //force hightest byte to 0
    unsigned short temp = 0;
    temp = decoded & 0x007f; //save 7 lower bits
    decoded >>= 7; //move the other 8 bits 7 to the left
    decoded |= temp << 8; //put the 7 saved bits to 8th position -> bits swapped :)
    decoded ^= cypher;

    QByteArray encoded;
    for(unsigned char fraction = 0; fraction < 3; fraction++) {
        temp = (decoded >> (5*fraction)) & 0b00011111; //get 5 bits
        temp += 48; //48 = code39 "0"
        if( temp > 57 ) //57 = code39 "9"
            temp += 8; //57+8=65 = code39 "A"
        encoded.append((char)temp);
    }
    return encoded;
}

unsigned short scanner::decode(QByteArray encoded, unsigned short cypher)
{
    if( encoded.size() != 3 )
        return 0;
    cypher &= 32767; //force hightest byte to 0
    unsigned short decoded = 0;
    unsigned short temp = 0;
    for(unsigned char fraction = 0; fraction < 3; fraction++) {
        temp = encoded.at(fraction); //get 5 bits
        if( temp > 57 ) //57 = code39 "9"
            temp -= 8; //57+8=65 = code39 "A"
        temp -= 48; //48 = code39 "0"
        decoded |= temp << (5*fraction);
    }

    decoded ^= cypher;
    temp = decoded & 0x00ff; //save 8 bits
    decoded >>= 8; //move the other 7 bits 8 to the left
    decoded |= temp << 7; //put the 8 saved bits to 7th position -> bits swapped :)
    return decoded;
}

#include <math.h>

QByteArray scanner::encodeLegacy(unsigned short decoded)
{
    double res,erg;
    erg =((decoded*3)+1018);
    res = round(erg/2);
    return ( QString::number((int) res).toAscii() );
}

unsigned short scanner::decodeLegacy(QByteArray encoded)
{
    double res;
    res = round(((QString::fromAscii(encoded).toInt()*2)-1018)/3) ;
    return ( (int) res );
}
