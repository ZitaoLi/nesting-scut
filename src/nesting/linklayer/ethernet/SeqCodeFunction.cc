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

#include "SeqCodeFunction.h"

namespace nesting {

Define_Module(SeqCodeFunction);
Define_Module(SeqEncodeFunction);
Define_Module(SeqDecodeFunction);

SeqCodeFunction::SeqCodeFunction() {
    // TODO Auto-generated constructor stub

}

SeqCodeFunction::~SeqCodeFunction() {
    // TODO Auto-generated destructor stub
}

SeqEncodeFunction::SeqEncodeFunction() {
    // TODO Auto-generated constructor stub

}

SeqEncodeFunction::~SeqEncodeFunction() {
    // TODO Auto-generated destructor stub
}

SeqDecodeFunction::SeqDecodeFunction() {
    // TODO Auto-generated constructor stub

}

SeqDecodeFunction::~SeqDecodeFunction() {
    // TODO Auto-generated destructor stub
}

void SeqCodeFunction::initialize(int stage) {
    if (stage == inet::INITSTAGE_LOCAL) {
        // Signals
//        encapPkSignal = registerSignal("encapPk");
//        decapPkSignal = registerSignal("decapPk");

        verbose = par("verbose");
        tagUntaggedFrames = par("tagUntaggedFrames");
//        pvid = par("pvid");

        totalFromHigherLayer = 0;
        WATCH(totalFromHigherLayer);

        totalFromLowerLayer = 0;
        WATCH(totalFromLowerLayer);

        totalEncap = 0;
        WATCH(totalEncap);

        totalDecap = 0;
        WATCH(totalDecap);
    } else if (stage == inet::INITSTAGE_LINK_LAYER) {
        if (par("registerProtocol").boolValue()) {
            registerService(inet::Protocol::ipv4, gate("in"), nullptr);
            registerProtocol(inet::Protocol::ipv4, nullptr, gate("out"));
            // TODO check if protocol is correct
        }
    }

}

void SeqEncodeFunction::handleMessage(cMessage* msg) {
    if (dynamic_cast<inet::Packet *>(msg) == nullptr){delete msg; return;}
    inet::Packet* packet = check_and_cast<inet::Packet*>(msg);

    EV_INFO << getFullPath() << ": Received " << packet << " from upper layer."
                       << endl;

    totalFromHigherLayer++;

    // TODO: Encapsulate R-Tag Header
    if (packet->findTag<VLANTagR>()) {
        auto rTag = packet->getTag<VLANTagR>();
        const auto& rTagHeader = inet::makeShared<nesting::Ieee8021qcbHeader>();
        rTagHeader->setUniqueID(rTag->getUniqueID());
        rTagHeader->setSeqNum(rTag->getSeqNum());
        packet->insertAtFront(rTagHeader);
        delete packet->removeTagIfPresent<VLANTagR>();
        // Statistics and logging
        EV_INFO << getFullPath() << ":Encapsulating higher layer packet `"
                       << packet->getName() << "' into R-Tag" << endl;
        totalEncap++;
    }

//    EV_TRACE << getFullPath() << ": Packet-length is "
//                    << packet->getByteLength() << " and Destination is "
//                    << packet->getTag<inet::MacAddressReq>()->getDestAddress()
//                    << " before sending packet to lower layer" << endl;

    send(packet, "out");
}

void SeqDecodeFunction::handleMessage(cMessage* msg) {
    if (dynamic_cast<inet::Packet *>(msg) == nullptr){delete msg; return;}
    inet::Packet* packet = check_and_cast<inet::Packet*>(msg);

    EV_INFO << getFullPath() << ": Received " << packet << " from lower layer."
                       << endl;

    totalFromLowerLayer++;

    // Decapsulate packet if it is a VLAN Tag, otherwise just insert default
    // values into the control information
//    EV_TRACE << getFullPath() << ": Packet-length is "
//                    << packet->getByteLength() << ", destination is "
//                    << packet->getTag<inet::MacAddressInd>()->getDestAddress();
    if (packet->hasAtFront<nesting::Ieee8021qcbHeader>()) {
        auto rTagHeader = packet->popAtFront<nesting::Ieee8021qcbHeader>();
        auto rTag = packet->addTagIfAbsent<VLANTagR>();
        rTag->setUniqueID(rTagHeader->getUniqueID());
        rTag->setSeqNum(rTagHeader->getSeqNum());
        EV_TRACE << ", StreamID is " << (int) rTag->getUniqueID() << " ";
        EV_TRACE << getFullPath() << ": Decapsulating packet and `"
                        << "' passing up contained packet `"
                        << packet->getName() << "' to higher layer" << endl;

        totalDecap++;
    } else {
        // TODO: if there is no R-Tag, discard this packet
        cancelAndDelete(packet);
    }

    EV_TRACE << " before sending packet up" << endl;

    // Send packet to upper layer
    send(packet, "out");
}


} /* namespace nesting */
