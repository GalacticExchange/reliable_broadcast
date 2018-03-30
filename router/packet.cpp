
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
    Packet::write<typeof(amount)>(buffer.begin(), offset, amount);
    offset += sizeof(amount);
    for (int len : lengths) {
        Packet::write<typeof(len)>(buffer.begin(), offset, len);
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
    auto amount = Packet::parse<int>(packet.get()->begin(), packet.get()->end(), offset);
    offset += sizeof(amount);

    vector<int> lengths;
    for (int i = 0; i < amount; i++) {

        auto len = Packet::parse<int>(packet.get()->begin(), packet.get()->end(), offset);
        lengths.push_back(len);

        offset += sizeof(len);
    }

    for (auto len : lengths) {
        vector<char> rawMessage = Packet::parse<vector<char> >(packet.get()->begin(), packet.get()->end(), offset);
        rawMessages.push_back(rawMessage);
        offset += len;
    }

    return make_shared<vector<vector<char>>>(rawMessages);
}

template<class T>
void Packet::write(vector<char>::iterator begin, size_t offset, const T &value)
{
    copy(reinterpret_cast<const char*>(&value),
         reinterpret_cast<const char*>(&value) + sizeof(T),
         &*begin + offset);
}

template<class T>
T Packet::parse(std::vector<char>::const_iterator begin, std::vector<char>::const_iterator end, size_t offset) {
    T value;
    copy(&*begin + offset, &*begin + offset + sizeof(T), reinterpret_cast<char*>(&value));
    return value;
}