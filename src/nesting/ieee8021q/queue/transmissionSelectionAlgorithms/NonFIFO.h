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

#ifndef NESTING_IEEE8021Q_QUEUE_TRANSMISSIONSELECTIONALGORITHMS_NONFIFO_H_
#define NESTING_IEEE8021Q_QUEUE_TRANSMISSIONSELECTIONALGORITHMS_NONFIFO_H_

#include "omnetpp.h"
#include "TSAlgorithm.h"
#include "../../Ieee8021qcbHeader_m.h"

using namespace omnetpp;

namespace nesting {

class NonFIFO: public TSAlgorithm{
private:
    GateController* gateController;

    unsigned int packetIndex = 0;

    // there is a packet in queue is ready to be sent
    bool readyToSend = false;

    unsigned int currentUniqueID = 0;

    unsigned int receivedUniqueID = 0;

    std::unique_ptr<Schedule<GateBitvector>> currentSchedule;
public:
    NonFIFO();
    virtual ~NonFIFO();

    virtual void initialize(int stage) override;

    virtual int numInitStages() const override;

    virtual void check();

    virtual void handlePacketEnqueuedEvent() override;

    virtual void handleRequestPacketEvent(uint64_t maxBits) override;

    virtual void handleGateStateSettedEvent() override;  // new

    virtual bool isEmpty(uint64_t maxBits);

    virtual void refreshDisplay() const override;
};

} /* namespace nesting */

#endif /* NESTING_IEEE8021Q_QUEUE_TRANSMISSIONSELECTIONALGORITHMS_NONFIFO_H_ */
