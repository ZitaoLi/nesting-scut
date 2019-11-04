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

#ifndef NESTING_LINKLAYER_ETHERNET_SEQCODEFUNCTION_H_
#define NESTING_LINKLAYER_ETHERNET_SEQCODEFUNCTION_H_

#include <omnetpp.h>

#include "inet/linklayer/common/Ieee802Ctrl.h"
#include "inet/linklayer/ieee8021q/Ieee8021qHeader_m.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "../common/VLANTag_m.h"
#include "../../ieee8021q/Ieee8021q.h"
#include "../../ieee8021q/Ieee8021qcbHeader_m.h"

using namespace omnetpp;

namespace nesting {

class SeqCodeFunction: public omnetpp::cSimpleModule {
private:
    /** Parameter from NED file. */
    bool tagUntaggedFrames;
    /** detailed prints for testing */
    bool verbose;
public:
    SeqCodeFunction();
    virtual ~SeqCodeFunction();
protected:
    /** Total amount of packets received from higher layer. */
    long totalFromHigherLayer;

    /** Total amount of packets received from lower layer. */
    long totalFromLowerLayer;

    /** Total amount of packets encapsulated. */
    long totalEncap;

    /** Total amount of packets decapsulated. */
    long totalDecap;

    virtual void initialize(int stage) override;

    virtual int numInitStages() const override {
        return inet::INITSTAGE_LINK_LAYER + 1;
    }
};

class SeqEncodeFunction: public SeqCodeFunction {
public:
    SeqEncodeFunction();
    virtual ~SeqEncodeFunction();
protected:
    virtual void handleMessage(cMessage* msg) override;
};

class SeqDecodeFunction: public SeqCodeFunction {
public:
    SeqDecodeFunction();
    virtual ~SeqDecodeFunction();
protected:
    virtual void handleMessage(cMessage* msg) override;
};

} /* namespace nesting */

#endif /* NESTING_LINKLAYER_ETHERNET_SEQCODEFUNCTION_H_ */
