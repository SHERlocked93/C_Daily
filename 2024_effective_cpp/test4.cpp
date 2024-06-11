#include <string>
#include <iostream>

typedef unsigned int UINT;

using namespace std;

inline void stationNum2StationId(UINT stationNum, UINT &stationId) {
    stationId = 0x09000000;
    const UINT lineId = stationNum / 100 % 100;
    const UINT stationSerial = stationNum % 100;
    stationId |= 0xFF & stationSerial;
    stationId |= 0xFF00 & (lineId << 8);
}

int main() {
    UINT sid;
    stationNum2StationId(6047, sid);
    cout << hex << sid << endl;
}
