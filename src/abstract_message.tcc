#ifndef ABSTRACT_MESSAGE_TEMPLATE_IMPLEMENTATION_H
#define ABSTRACT_MESSAGE_TEMPLATE_IMPLEMENTATION_H

template<class T>
T AbstractMessage::parse(std::vector<char>::const_iterator begin,
                         std::vector<char>::const_iterator end,
                         size_t &offset)
{
    T value;
    std::copy(&*begin + offset, &*begin + offset + sizeof(T), reinterpret_cast<char*>(&value));
    offset += sizeof(T);
    return value;
}

template<class T>
void AbstractMessage::write(std::vector<char>::iterator &begin, const T &value)
{
    std::copy(reinterpret_cast<const char*>(&value),
              reinterpret_cast<const char*>(&value) + sizeof(T),
              &*begin);
    begin += sizeof(T);
}

#endif // ABSTRACT_MESSAGE_TEMPLATE_IMPLEMENTATION_H
