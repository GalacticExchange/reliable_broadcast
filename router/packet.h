#ifndef BROADCAST_PACKET_H
#define BROADCAST_PACKET_H

#include <vector>
#include <memory>
#include "../src/message.h"

using namespace std;

class Packet {

public:

    /**
     * @param encodedMsgs - encoded messages array
     * @return encoded packet -> {amount, len[], data[]}
     */
    static shared_ptr<const vector<char>> createPacket(vector<vector<char> > &encodedMsgs);

    /**
     * @param packet
     * @return encoded messages array
     */
    static shared_ptr<vector<vector<char>> > parsePacket(shared_ptr<const vector<char>> packet);

};

#endif //BROADCAST_PACKET_H
