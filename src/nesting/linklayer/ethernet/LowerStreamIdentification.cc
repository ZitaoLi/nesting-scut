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

#include "LowerStreamIdentification.h"
#include <exception>


namespace nesting {

Define_Module(LowerStreamIdentification);

LowerStreamIdentification::LowerStreamIdentification() {
    // TODO Auto-generated constructor stub

}

LowerStreamIdentification::~LowerStreamIdentification() {
    // TODO Auto-generated destructor stub
}

void LowerStreamIdentification::handleMessage(cMessage* msg) {
    if (dynamic_cast<inet::Packet *>(msg) == nullptr){delete msg; return;}
    inet::Packet* packet = check_and_cast<inet::Packet*>(msg);

//    bool t = packet->hasAt<nesting::Ieee8021qcbHeader>(inet::b(208), inet::b(48));

    if (packet->findTag<VLANTagR>()) {
       send(packet, "outFRER");
       return;
   } else if (packet->findTag<VLANTagInd>()) { // 临时的解决办法,底下那个有bug
       auto vlanTag = packet->getTag<VLANTagInd>();
       if (vlanTag->getPcp() == 7) {
           send(packet, "outFRER");
           return;
       } else {
           send(packet, "outNonFRER");
           return;
       }
   } else {
       send(packet, "outNonFRER");
       return;
   }

//    if (packet->findTag<VLANTagR>()) {
//        send(msg, "outFRER");
//        return;
//    } else if (packet->hasAtFront<nesting::Ieee8021qcbHeader>(inet::b(48))) {
//        send(msg, "outFRER");
//        return;
//    } else {
//        send(msg, "outNonFRER");
//        return;
//    }

//    if (packet->findTag<VLANTagR>()) {
//        send(msg, "outFRER");
//        return;
//    } else {
//        try {
//            packet->hasAtFront<nesting::Ieee8021qcbHeader>();
//            send(msg, "outFRER");
//        } catch (std::exception &e) {
//            send(msg, "outNonFRER");
//        }
//        return;
//    }
}

} /* namespace nesting */
