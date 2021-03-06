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

#include "LengthAwareQueue.h"

namespace nesting {

Define_Module(LengthAwareQueue);

LengthAwareQueue::~LengthAwareQueue() {
    cancelEvent(&requestPacketMsg);
    while (!queue.isEmpty()) {
        delete queue.pop();
    }
    queue.clear();
}

void LengthAwareQueue::initialize() {
    rcvdPkSignal = registerSignal("rcvdPk");
    enqueuePkSignal = registerSignal("enqueuePk");
    dequeuePkSignal = registerSignal("dequeuePk");
    dropPkByQueueSignal = registerSignal("dropPkByQueue");
    queueingTimeSignal = registerSignal("queueingTime");
    queueLengthSignal = registerSignal("queueLength");

    queue.setName(par("queueName"));
    availableBufferCapacity = par("bufferCapacity");
    expressQueue = par("expressQueue");
    WATCH(numPacketsReceived);
    WATCH(numPacketsDropped);
    WATCH(numPacketsEnqueued);
    WATCH(availableBufferCapacity);

    // module references
    tsAlgorithm = getModuleFromPar<TSAlgorithm>(
            par("transmissionSelectionAlgorithmModule"), this);

    // statistics
    emit(queueLengthSignal, queue.getLength());
}

void LengthAwareQueue::handleMessage(cMessage* msg) {
    if (msg->isSelfMessage()) {
        if (msg == &requestPacketMsg) {
            handleRequestPacketEvent(maxTransmittableBits, requestPacketIndex);
        }
    } else {
        cPacket* packet = check_and_cast<cPacket*>(msg);
        emit(rcvdPkSignal, packet);
        numPacketsReceived++;
        enqueue(packet);
    }
}

void LengthAwareQueue::enqueue(cPacket* packet) {
    if (availableBufferCapacity >= packet->getBitLength()) {
        emit(enqueuePkSignal, packet);
        numPacketsEnqueued++;
        queue.insert(packet);
        availableBufferCapacity -= packet->getBitLength();
        handlePacketEnqueuedEvent(packet);
    } else {
        emit(dropPkByQueueSignal, packet);
        numPacketsDropped++;
        handlePacketEnqueuedEvent(packet);
        delete packet;
    }
    emit(queueLengthSignal, queue.getLength());
}

cPacket* LengthAwareQueue::dequeue() {
    if (queue.isEmpty()) {
        return nullptr;
    }

    cPacket* packet = static_cast<cPacket*>(queue.pop());
    availableBufferCapacity += packet->getBitLength();
    
    emit(queueLengthSignal, queue.getLength());

    return packet;
}

cPacket* LengthAwareQueue::dequeueWithIndex(unsigned int index) {
    if (queue.isEmpty()) {
        return nullptr;
    }

    cPacket* packet = static_cast<cPacket*>(queue.get(index));
    queue.remove(packet);
    availableBufferCapacity += packet->getBitLength();

    emit(queueLengthSignal, queue.getLength());

    return packet;
}

void LengthAwareQueue::handleRequestPacketEvent(uint64_t maxBits, unsigned int index) {
    ASSERT(!isEmpty(maxBits));

    if (index == 0) {
        cPacket* nextPacket = static_cast<cPacket*>(queue.front());  // FIFO
        EV_TRACE << getFullPath() << ": Packet requested with max length of "
                        << maxBits << "bits. Next packet has "
                        << static_cast<uint64_t>(nextPacket->getBitLength())
                        << "bits." << endl;

        cPacket* packetToSend = dequeue();

        emit(dequeuePkSignal, packetToSend);
        emit(queueingTimeSignal, simTime() - packetToSend->getArrivalTime());

        send(packetToSend, "out");
    } else {
        cPacket* nextPacket = static_cast<cPacket*>(queue.get(index));  // non-FIFO
        EV_TRACE << getFullPath() << ": Packet requested with max length of "
                        << maxBits << "bits. Next packet has "
                        << static_cast<uint64_t>(nextPacket->getBitLength())
                        << "bits." << endl;

        cPacket* packetToSend = dequeueWithIndex(index);

        emit(dequeuePkSignal, packetToSend);
        emit(queueingTimeSignal, simTime() - packetToSend->getArrivalTime());

        send(packetToSend, "out");
    }
}

void LengthAwareQueue::handlePacketEnqueuedEvent(cPacket* packet) {
    EV_TRACE << getFullPath() << ": Handle packet-enqueued event." << endl;
    tsAlgorithm->packetEnqueued(); // Notifies the transmission-selection-algorithm, that a packet got ready for transmission on the input module.
}

bool LengthAwareQueue::isEmpty(uint64_t maxBits) {
    if (queue.isEmpty()) {
        return true;
    }

    cPacket* nextPacket = static_cast<cPacket*>(queue.front());

    // next packet will be sent only when the packet size < maxBits
    // so if next packet size > maxBits, it'll return true, otherwise return false
    return static_cast<uint64_t>(nextPacket->getBitLength() + 240) > maxBits; // add 240 bits to account for headers (30 bytes * 8)
}

void LengthAwareQueue::requestPacket(uint64_t maxBits, unsigned int index) {
    Enter_Method("requestPacket(maxBits)");
    maxTransmittableBits = maxBits;
    requestPacketIndex = index;
    cancelEvent(&requestPacketMsg);
    scheduleAt(simTime(), &requestPacketMsg);
}

bool LengthAwareQueue::isExpressQueue() {
    return expressQueue;
}

cPacket* LengthAwareQueue::getPacketFromQueue(int i) {
    if (queue.isEmpty()) return nullptr;
    cPacket* packet = static_cast<cPacket*>(queue.get(i));
    return packet;
}

unsigned int LengthAwareQueue::getPacketNum() {
    if (queue.isEmpty()) return 0;
    return queue.getLength();
}
}
// namespace nesting
