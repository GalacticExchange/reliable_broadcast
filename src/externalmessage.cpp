#include <memory>
#include <vector>

using std::make_shared;
using std::vector;

#include "externalmessage.h"

//ExternalMessage::ExternalMessage(vector<char>::iterator begin, vector<char>::iterator end)
//{
//    mMessage = make_shared<vector<char>>(*begin ? begin : begin + 1, end);
//}

//std::shared_ptr<const std::vector<char> > ExternalMessage::getMessagePtr() const
//{
//    return mMessage;
//}

//const std::vector<char> &ExternalMessage::getMessage() const
//{
//    return *mMessage;
//}

//Message::MessageType ExternalMessage::getType() const
//{
//    return MESSAGE;
//}
