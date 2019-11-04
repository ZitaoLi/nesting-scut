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

#ifndef NESTING_IEEE8021Q_QUEUE_BASERECOVERYFUNCTION_H_
#define NESTING_IEEE8021Q_QUEUE_BASERECOVERYFUNCTION_H_

#include <omnetpp.h>
#include <list>
#include <unordered_map>
#include <map>
#include "inet/common/packet/Packet.h"
#include "../Ieee8021q.h"
#include "../../linklayer/common/VLANTagR_m.h"

using namespace omnetpp;

namespace nesting {

struct FrameRecordEntry {
    int lower;
    int upper;
};

typedef std::list<FrameRecordEntry> FRAME_RECORDER;

class BaseRecoveryFunction: public omnetpp::cSimpleModule {
private:
//    std::list<FrameRecordEntry> frameRecorder;
    std::unordered_map<int, FRAME_RECORDER> STREAM_SEQNUM_MAP;
public:
    BaseRecoveryFunction();
    virtual ~BaseRecoveryFunction();
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    bool checkReception(FRAME_RECORDER &fr, int seqNum);
};

} /* namespace nesting */

#endif /* NESTING_IEEE8021Q_QUEUE_BASERECOVERYFUNCTION_H_ */
