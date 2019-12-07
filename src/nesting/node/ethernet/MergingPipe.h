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

#ifndef NESTING_NODE_ETHERNET_MERGINGPIPE_H_
#define NESTING_NODE_ETHERNET_MERGINGPIPE_H_

#include <omnetpp.h>
#include <unordered_map>
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/chunk/Chunk.h"
#include "../../ieee8021q/Ieee8021qcbHeader_m.h"
#include "../../linklayer/common/VLANTagR_m.h"

using namespace omnetpp;

namespace nesting {

class MergingPipe: public cSimpleModule {
private:
    std::unordered_map<unsigned int, unsigned int> dictFlowPort;
    std::unordered_map<unsigned int, float> dictFlowPER;
public:
    MergingPipe();
    virtual ~MergingPipe();
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
private:
    virtual void processPacketFromLowerLevel(inet::Packet *packet);
    virtual void processPacketFromHigherLevel(inet::Packet *packet);
};

} /* namespace nesting */

#endif /* NESTING_NODE_ETHERNET_MERGINGPIPE_H_ */
