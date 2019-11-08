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

#include "NonFIFO.h"

namespace nesting {

Define_Module(NonFIFO);

NonFIFO::NonFIFO() {
    // TODO Auto-generated constructor stub

}

NonFIFO::~NonFIFO() {
    // TODO Auto-generated destructor stub
}

void NonFIFO::initialize(int stage) {
    if (stage == INITSTAGE_LOCAL) {
        TSAlgorithm::initialize();
        gateController = getModuleFromPar<GateController>(par("gateControllerModule"), this);
    } else if (stage == INITSTAGE_LINK_LAYER) {
        // do nothing
    }
}

int NonFIFO::numInitStages() const {
    return INITSTAGE_LINK_LAYER + 1;
}

void NonFIFO::check() {
    unsigned int scheduleIndex = gateController->scheduleIndex;
    scheduleIndex = (scheduleIndex - 1) % gateController->currentSchedule->size();
    if (scheduleIndex < 0) {
        scheduleIndex = (gateController->currentSchedule->size() - 1) % gateController->currentSchedule->size();
    }
//    unsigned int uniqueID = gateController->currentSchedule->getUniqueID(scheduleIndex);
    currentUniqueID = gateController->currentSchedule->getUniqueID(scheduleIndex);

    int pktNum = queue->getPacketNum();
    for (int i = 0; i < pktNum; i++) {
        if (dynamic_cast<inet::Packet *>(queue->getPacketFromQueue(i)) == nullptr){
            ASSERT(false);
        }
        Packet* packet = check_and_cast<Packet*>(queue->getPacketFromQueue(i));
        ASSERT(packet->hasAtFront<nesting::Ieee8021qcbHeader>());
        auto rTagHeader = packet->peekAtFront<nesting::Ieee8021qcbHeader>();
        if (currentUniqueID == rTagHeader->getUniqueID()) {
            readyToSend = true;
            break;
        } else {
            readyToSend = false;
        }
    }
}

void NonFIFO::handlePacketEnqueuedEvent() {
    unsigned int scheduleIndex = gateController->scheduleIndex;
        scheduleIndex = (scheduleIndex - 1) % gateController->currentSchedule->size();
        if (scheduleIndex < 0) {
            scheduleIndex = (gateController->currentSchedule->size() - 1) % gateController->currentSchedule->size();
        }
    currentUniqueID = gateController->currentSchedule->getUniqueID(scheduleIndex);

    int pktNum = queue->getPacketNum();
    for (int i = 0; i < pktNum; i++) {
        if (dynamic_cast<inet::Packet *>(queue->getPacketFromQueue(i)) == nullptr){
            ASSERT(false);
        }
        Packet* packet = check_and_cast<Packet*>(queue->getPacketFromQueue(i));
        ASSERT(packet->hasAtFront<nesting::Ieee8021qcbHeader>());
        auto rTagHeader = packet->peekAtFront<nesting::Ieee8021qcbHeader>();
        unsigned int uniqueID = rTagHeader->getUniqueID();
        receivedUniqueID = uniqueID;

        EV_TRACE << "received TSN packet belongs to flow [" << uniqueID << "]" << endl;

        if (currentUniqueID == receivedUniqueID) {
            readyToSend = true;
            packetIndex = i;
            TSAlgorithm::handlePacketEnqueuedEvent();
        } else {
            readyToSend = false;
        }
    }
}

void NonFIFO::handleRequestPacketEvent(uint64_t maxBits) {
    EV_TRACE << getFullPath() << ": Handle request-packet event (" << maxBits << " bits)." << endl;

    unsigned int scheduleIndex = gateController->scheduleIndex;
    unsigned int uniqueID = gateController->currentSchedule->getUniqueID(scheduleIndex);

    int pktNum = queue->getPacketNum();
    for (int i = 0; i < pktNum; i++) {
        if (dynamic_cast<inet::Packet *>(queue->getPacketFromQueue(i)) == nullptr){
            ASSERT(false);
        }
        Packet* packet = check_and_cast<Packet*>(queue->getPacketFromQueue(i));
        ASSERT(packet->hasAtFront<nesting::Ieee8021qcbHeader>());
        auto rTagHeader = packet->peekAtFront<nesting::Ieee8021qcbHeader>();
        if (currentUniqueID == rTagHeader->getUniqueID()) {
            packetIndex = i;
            queue->requestPacket(maxBits, packetIndex);
            break;
        }
    }
}

bool NonFIFO::isEmpty(uint64_t maxBits) {
    check();
    return (queue->isEmpty(maxBits) || !readyToSend);
}

void NonFIFO::handleGateStateSettedEvent() {
    int pktNum = queue->getPacketNum();
    if (pktNum != 0) {
        handlePacketEnqueuedEvent();
    }
}

void NonFIFO::refreshDisplay() const {
    char buf[256];
    sprintf(buf, "non-fifo\ncurrentUniqueID: %d\nreceivedUniqueID: %d\nreadyToSend: %s",
            static_cast<int>(currentUniqueID),
            static_cast<int>(receivedUniqueID),
            readyToSend ? "true" : "false");
    getDisplayString().setTagArg("t", 0, buf);
}

} /* namespace nesting */
