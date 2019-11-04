//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "BaseRecoveryFunction.h"

namespace nesting {

Define_Module(BaseRecoveryFunction);

BaseRecoveryFunction::BaseRecoveryFunction() {
    // TODO Auto-generated constructor stub

}

BaseRecoveryFunction::~BaseRecoveryFunction() {
    // TODO Auto-generated destructor stub
}

void BaseRecoveryFunction::initialize() {
    return;
}

void BaseRecoveryFunction::handleMessage(cMessage *msg) {
    inet::Packet *packet = check_and_cast<inet::Packet *>(msg);

    // switch ingoing VLAN Tag to outgoing Tag
//    auto vlanTagR = packet->removeTag<VLANTagR>();
    auto vlanTagR = packet->getTag<VLANTagR>();
    int uniqueID = vlanTagR->getUniqueID();
    int seqNum = vlanTagR->getSeqNum();

//    delete vlanTagR;

    EV_TRACE << "receive Packet(StreamID:" << uniqueID << ", seqNum:" << seqNum << ")" <<endl;

    if (STREAM_SEQNUM_MAP.find(uniqueID) != STREAM_SEQNUM_MAP.end()) {
        FRAME_RECORDER fr = STREAM_SEQNUM_MAP[uniqueID];
        bool isReception = checkReception(fr, seqNum);
        auto it = STREAM_SEQNUM_MAP.find(uniqueID);
        if(it != STREAM_SEQNUM_MAP.end()){
//            it-> = new_value;
            STREAM_SEQNUM_MAP[uniqueID] = fr;
        }
        if (isReception == true) {
            cancelAndDelete(msg);
            EV_TRACE << "filter Packet(StreamID:" << uniqueID << ", seqNum:" << seqNum << ")" <<endl;
        } else {
            send(msg, "out");
            EV_TRACE << "pass Packet(StreamID:" << uniqueID << ", seqNum:" << seqNum << ")" <<endl;
        }
    } else {
        // create and insert first recorder
        FRAME_RECORDER fr;
        FrameRecordEntry first = {seqNum, seqNum};
        fr.push_back(first);
        STREAM_SEQNUM_MAP.insert(std::pair<int, FRAME_RECORDER> {uniqueID, fr});
        EV_TRACE << "pass Packet(StreamID:" << uniqueID << ", seqNum:" << seqNum << ")" <<endl;
        send(msg, "out");
    }
}

bool BaseRecoveryFunction::checkReception(FRAME_RECORDER &fr, int seqNum) {
    std::list<FrameRecordEntry>::iterator it = fr.begin();
//    bool idReception = false;
//    if (it == fr.end())
//    {
//        FrameRecordEntry first = {seqNum, seqNum};
//        fr.push_back(first);
////        send(msg);
//        return false;
//    }
    while (it != fr.end())
    {
        int lower = (*it).lower;
        int upper = (*it).upper;
        if (seqNum >= lower && seqNum <= upper)
        {
//            cancelAndDelete (msg);
            return true;
//            isReception = true;
        } else if (seqNum == (upper + 1))
        {
            ((*it).upper)++;
            std::list<FrameRecordEntry>::iterator old = it;
            if (++it != fr.end())
            {
                if (seqNum == (*it).lower - 1)
                {
                    (*it).lower = lower;
                    it = fr.erase(old);
                }
            }
//            send(msg);
            return false;
        } else if (seqNum >= (upper + 2))
        {
            if (++it != fr.end())
            {
                if (seqNum == (*it).lower - 1)
                {
                    (*it).lower--;
                } else if(seqNum <= (*it).lower - 2)
                {
                    FrameRecordEntry mid = {seqNum, seqNum};
                    fr.insert(it, mid);
                } else
                {
                    continue;
                }

            } else
            {
                FrameRecordEntry last = {seqNum, seqNum};
                fr.push_back(last);
            }
//            send(msg);
            return false;
        } else {
            return true;
//            isReception = true;
        }
    }

//    return isReception;
    return true;
}

} /* namespace nesting */
