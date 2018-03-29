
#include "packet.h"

using namespace std;

shared_ptr<const vector<char>> Packet::createPacket(vector<vector<char>> &encodedMsgs) {
    vector<char> packet;

    auto amount = static_cast<int>(encodedMsgs.size());
    vector<int> lengths;

    for (const auto &msg: encodedMsgs) {
        packet.insert(packet.begin(), msg.begin(), msg.end());
        lengths.push_back(static_cast<int &&>(msg.size()));
    }
    size_t total_size = sizeof(amount) +
                        lengths.size() * sizeof(int) +
                        packet.size();

    vector<char> buffer(total_size);
    size_t offset = 0;
    Message::write<typeof(amount)>(buffer.begin(), offset, amount);
    offset += sizeof(amount);
    for (int len : lengths) {
        Message::write<typeof(len)>(buffer.begin(), offset, len);
        offset += sizeof(len);
    }
    copy(packet.begin(), packet.end(), buffer.begin() + offset);

//    buffer[offset] = static_cast<char>(mType);
//    offset += 1;
//    copy(mData.begin(), mData.end(), buffer.begin() + offset);


    return make_shared<vector<char> >(*buffer.begin() ? buffer.begin() : buffer.begin() + 1,
                                      buffer.begin() + buffer.size());
}

shared_ptr<vector<vector<char>>> Packet::parsePacket(shared_ptr<const vector<char>> packet) {
    vector<vector<char> > rawMessages;

    size_t offset = 0;
    auto amount = Message::parse<int>(packet.get()->begin(), packet.get()->end(), offset);
    offset += sizeof(amount);

    vector<int> lengths;
    for (int i = 0; i < amount; i++) {

        auto len = Message::parse<int>(packet.get()->begin(), packet.get()->end(), offset);
        lengths.push_back(len);

        offset += sizeof(len);
    }

    for (auto len : lengths) {
        vector<char> rawMessage = Message::parse<vector<char> >(packet.get()->begin(), packet.get()->end(), offset);
        rawMessages.push_back(rawMessage);
        offset += len;
    }

    return make_shared(rawMessages);
}
