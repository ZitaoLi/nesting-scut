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

#include "StatisticsApp.h"

namespace nesting {

Define_Module(StatisticsApp);

StatisticsApp::StatisticsApp() {
    // TODO Auto-generated constructor stub

}

StatisticsApp::~StatisticsApp() {
    // TODO Auto-generated destructor stub
}

void StatisticsApp::initialize() {
    WATCH(receivedPktNum);
    WATCH(sentPktNum);

    flowStatisticsApp = getModuleFromPar<FlowStatisticsApp>(par("flowStatisticsApp"), this);

    cXMLElement* xml = par("flows").xmlValue();
    loadXml(xml);
}

void StatisticsApp::handleMessage(cMessage *msg) {
    if (dynamic_cast<inet::Packet *>(msg) == nullptr){delete msg; return;}
        inet::Packet* packet = check_and_cast<inet::Packet*>(msg);

    if (packet->arrivedOn("fromHigherLayer$i")) {
        processPacketFromHigherLevel(packet);
    } else if (packet->arrivedOn("fromLowerLayer$i")) {
        processPacketFromLowerLevel(packet);
    }
}

void StatisticsApp::loadXml(cXMLElement* xml) {
    for (cXMLElement* flowxml : xml->getChildren()) {
        unsigned int uniqueID = atoi(flowxml->getAttribute("id"));
        std::pair<unsigned int, unsigned int> item1 = {uniqueID, 0};
        dictFlowReceivedPktNum.insert(item1);
        std::pair<unsigned int, unsigned int> item2 = {uniqueID, 0};
        dictFlowSentPktNum.insert(item2);
    }
}

void StatisticsApp::processPacketFromLowerLevel(inet::Packet *packet) {
    if (packet->findTag<VLANTagR>()) {
        auto rTag = packet->getTag<VLANTagR>();
        unsigned int uniqueID = rTag->getUniqueID();
        EV_INFO << getFullPath() << " received TSN-flow: " << uniqueID << endl;
        dictFlowReceivedPktNum.at(uniqueID) += 1;
        flowStatisticsApp->dictFlowReceivedPktNum.at(uniqueID) += 1;
        receivedPktNum += 1;
    }
    cancelAndDelete(packet);
}

void StatisticsApp::processPacketFromHigherLevel(inet::Packet *packet) {
    if (packet->findTag<VLANTagR>()) {
        auto rTag = packet->getTag<VLANTagR>();
        unsigned int uniqueID = rTag->getUniqueID();
        EV_INFO << getFullPath() << " sent TSN-flow: " << uniqueID << endl;
        dictFlowSentPktNum.at(uniqueID) += 1;
        flowStatisticsApp->dictFlowSentPktNum.at(uniqueID) += 1;
        sentPktNum += 1;
    }
    cancelAndDelete(packet);
}

} /* namespace nesting */
