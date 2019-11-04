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

#include "../queue/TransmissionSelection.h"

namespace nesting {

Define_Module(TransmissionSelection);

TransmissionSelection::TransmissionSelection() {
    requestPacketMsg.setSchedulingPriority(selfMessageSchedulingPriority);
    packetEnqueuedMsg.setSchedulingPriority(selfMessageSchedulingPriority);
    WATCH(packetRequestedFromUs);
    WATCH(packetRequestedFromInputs);
}

TransmissionSelection::~TransmissionSelection() {
    cancelEvent(&requestPacketMsg);
    cancelEvent(&packetEnqueuedMsg);
}

void TransmissionSelection::initialize() {
    // Get transmission gate vector module
    TransmissionGate* tgModule = getModuleFromPar<TransmissionGate>(
            par("transmissionGateVectorModule"), this);
    // Iterate through all sibling modules
    auto it = cModule::SubmoduleIterator(tgModule->getParentModule());
    for (; !it.end(); it++) {
        cModule* subModule = *it;
        // If module belongs to transmission gate vector module, add it to
        // transmission-gate data-structure for easy access later on.
        if (subModule->isName(tgModule->getName())) {
            TransmissionGate* tg = check_and_cast<TransmissionGate*>(subModule);
            tGates.push_back(tg);
        }
    }
}

void TransmissionSelection::handleMessage(cMessage* msg) {
    if (msg->isSelfMessage()) {
        if (msg == &packetEnqueuedMsg) {
            handlePacketEnqueuedEvent();
        } else if (msg == &requestPacketMsg) {
            handleRequestPacketEvent();
        }
    } else {
        int gateId = -1;
        for (unsigned int i = 0; i < tGates.size(); ++i) {
            if (msg->arrivedOn("in", i)) {
                gateId = i;
                break;
            }
        }
        TransmissionGate* transmissionGate = tGates.at(gateId);
        if (par("verbose")) {
            EV_DETAIL << getFullName() << ": msg arrived at gate: " << gateId
                             << " which is express: "
                             << transmissionGate->isExpressQueue() << endl;
        }
        ASSERT(packetRequestedFromUs);
        // if a packet has been sent in this module (transmission-selection module),
        // then set "packetRequestedFromUs" to false
        packetRequestedFromUs = false;
        if (transmissionGate->isExpressQueue()) {
            send(msg, "eOut");
        } else {
            send(msg, "pOut");
        }
    }
}

void TransmissionSelection::refreshDisplay() const {
    char buf[80];
    sprintf(buf, "packetRequestedFromUs: %s packetRequestedFromInputs: %s",
            (packetRequestedFromUs ? "true" : "false"), (packetRequestedFromInputs ? "true" : "false"));
    getDisplayString().setTagArg("t", 0, buf);
}

bool TransmissionSelection::schedulePacket() {
    /*
     * TODO fix bugs here
     */

    //Try to request express packet
    for (auto it = tGates.rbegin(); it != tGates.rend(); it++) {
        TransmissionGate* transmissionGate = *it;
        if (transmissionGate->isExpressQueue()
                && !transmissionGate->isEmpty()) {
            transmissionGate->requestPacket(); // request a packet from transmission gate
            return true;
        }
    }
    //Try to request any packet
    for (auto it = tGates.rbegin(); it != tGates.rend(); it++) {
        TransmissionGate* transmissionGate = *it;
        if (!transmissionGate->isEmpty()) {
            transmissionGate->requestPacket(); // request a packet from transmission gate
            return true;
        }
    }
    return false;
}

bool TransmissionSelection::schedulePacket2() {
    //Try to request express packet
    for (auto it = tGates.rbegin(); it != tGates.rend(); it++) {
        TransmissionGate* transmissionGate = *it;
        unsigned int pktInQueueNum = transmissionGate->getPakcetNum();
        bool flag = false;
        std::cout << getFullPath() << ": pktInQueueNum: " << pktInQueueNum << std::endl;
        for (int i = 0; i < pktInQueueNum; i++) {
            if (transmissionGate->isExpressQueue() && !transmissionGate->isEmpty()) {
                transmissionGate->requestPacket(); // request a packet from transmission gate
                flag = true;
            }
        }
        if (flag == true) return true;
    }
    //Try to request any packet
    for (auto it = tGates.rbegin(); it != tGates.rend(); it++) {
        TransmissionGate* transmissionGate = *it;
        unsigned int pktInQueueNum = transmissionGate->getPakcetNum();
        bool flag = false;
        for (int i = 0; i < pktInQueueNum; i++) {
            if (!transmissionGate->isEmpty()) {
                transmissionGate->requestPacket(); // request a packet from transmission gate
                flag = true;
            }
        }
        if (flag == true) return true;
    }
    return false;
}

void TransmissionSelection::handleRequestPacketEvent() {
    EV_TRACE << getFullPath() << ": Handle request-packet-event." << endl;

    // Precondition: The Mac module must not request more than one packet at a
    // time. This implies the Mac module can not operate in burst mode or the
    // like.
    if (packetRequestedFromUs) {
        throw cRuntimeError(
                "TransmissionSelection module only supports one packet request at a +"
                        "time.");
    }

    // Invariant: Internal packet request flags are false
    ASSERT(!packetRequestedFromUs && !packetRequestedFromInputs);

    packetRequestedFromUs = true;
    packetRequestedFromInputs = true;

    // Try to request a packet from inputs if possible
    bool success = schedulePacket();
//    bool success = schedulePacket2();
    if (success) {
        // if a packet has been received successfully from input module (transmission-gate module),
        // then set "packetRequestedFromInputs" to false
        packetRequestedFromInputs = false;
    }
}

void TransmissionSelection::handlePacketEnqueuedEvent() {
    EV_TRACE << getFullPath() << ": Handle packet-enqueued-event." << endl;

    if (packetRequestedFromInputs) {
        // Try to request a packet from inputs if possible
        bool success = schedulePacket();
//        bool success = schedulePacket2();
        if (success) {
            // if a packet has been received successfully from input module (transmission-gate module),
            // then set "packetRequestedFromInputs" to false
            packetRequestedFromInputs = false;
        }
    } else if (!packetRequestedFromUs) {
        // if fail to request a packet from inputs
        // notify passive-queue-listener a packet is still in queue
        notifyPacketEnqueued();
    }
}

void TransmissionSelection::packetEnqueued(TransmissionGate* transmissionGate) {
    Enter_Method("packetEnqueued()");

    cancelEvent(&packetEnqueuedMsg);
    scheduleAt(simTime(), &packetEnqueuedMsg);
}

// this method will be called by PassiveQueueListener
void TransmissionSelection::requestPacket() {
    Enter_Method("requestPacket()");

    cancelEvent(&requestPacketMsg);
    scheduleAt(simTime(), &requestPacketMsg);
}

int TransmissionSelection::getNumPendingRequests() {
    return packetRequestedFromUs ? 1 : 0;
}

bool TransmissionSelection::isEmpty() {
    for (auto transmissionGate : tGates) {
        if (!transmissionGate->isEmpty()) {
            return false;
        }
    }
    return true;
}

void TransmissionSelection::removePendingRequests() {
    packetRequestedFromUs = false;
    packetRequestedFromInputs = false;
}

void TransmissionSelection::clear() {
    throw cRuntimeError("Unsupported operation");
}

cMessage* TransmissionSelection::pop() {
    throw cRuntimeError("Unsupported operation");
}

void TransmissionSelection::addListener(IPassiveQueueListener *listener) {
    listeners.push_back(listener);
}

void TransmissionSelection::removeListener(IPassiveQueueListener *listener) {
    listeners.erase(std::remove(listeners.begin(), listeners.end(), listener),
            listeners.end());
}

void TransmissionSelection::notifyPacketEnqueued() {
    for (auto listener : listeners) {
        listener->packetEnqueued(this);
    }
}

bool TransmissionSelection::hasExpressPacketEnqueued() {
    for (auto transmissionGate : tGates) {
        if (transmissionGate->isExpressQueue()
                && !transmissionGate->isEmpty()) {
            return true;
        }
    }
    return false;
}

} // namespace nesting
