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

#include "MergingPipe.h"

namespace nesting {

Define_Module(MergingPipe);

MergingPipe::MergingPipe() {
    // TODO Auto-generated constructor stub

}

MergingPipe::~MergingPipe() {
    // TODO Auto-generated destructor stub
}

void MergingPipe::initialize() {
    // TODO load xml
}

void MergingPipe::handleMessage(cMessage *msg) {
    if (dynamic_cast<inet::Packet *>(msg) == nullptr){delete msg; return;}
    inet::Packet* packet = check_and_cast<inet::Packet*>(msg);

    int hostInNum = gateSize("hostIn");
    if (packet->arrivedOn("hostIn")) {
        send(packet, "channelOut");
    } else if (packet->arrivedOn("channelIn")) {
        // submit to target host
//        processPacketFromLowerLevel(packet);
        send(packet, "toStatistics$o");
    }
}

void MergingPipe::processPacketFromLowerLevel(inet::Packet *packet) {
    // length of phyHeader = 8Byte, length of EthernetHeader = 14Byte, length of IEEE8021QHeader = 4Byte
    inet::b offset = inet::b(8 * 8 + 14 * 8 + 4 * 8);
    // length of IEEE8021QCBHeader = 6Byte
    inet::b rTagOffset = inet::b(6 * 8);
    inet::Chunk::enableImplicitChunkSerialization = true; // enable implication chunk serialization
    if (packet->hasAt<nesting::Ieee8021qcbHeader>(offset, rTagOffset)) {
        auto rTagHeader = packet->peekDataAt<nesting::Ieee8021qcbHeader>(offset, rTagOffset);
        unsigned int uniqueID = rTagHeader->getUniqueID();
    } else {
        cancelAndDelete(packet);
    }
    inet::Chunk::enableImplicitChunkSerialization = false; // disenable implication chunk serialization
    delete packet;
}

} /* namespace nesting */
