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

#include "FlowStatisticsApp.h"

namespace nesting {

Define_Module(FlowStatisticsApp);

FlowStatisticsApp::FlowStatisticsApp() {
    // TODO Auto-generated constructor stub

}

FlowStatisticsApp::~FlowStatisticsApp() {
    for(auto item: dictFlowSentPktNum) {
        result.at(item.first)[0] += item.second;
    }
    for(auto item: dictFlowReceivedPktNum) {
        result.at(item.first)[1] += item.second;
    }
    // save result
    const char* filename = par("statisticsResultFilename").stringValue();
    std::ofstream target;
    target.open(filename);
    for(auto item: result) {
        std::cout << "FlowID = " << item.first <<
                " sent packet = " << item.second[0] <<
                " received packet = " << item.second[1] << std::endl;
        target << item.first << "," << item.second[0] << "," << item.second[1] << std::endl;
    }
    target.close();
}

void FlowStatisticsApp::initialize() {
    cXMLElement* xml = par("flows").xmlValue();
    loadXml(xml);
}

void FlowStatisticsApp::handleMessage(cMessage *msg) {

}

void FlowStatisticsApp::loadXml(cXMLElement* xml) {
    for (cXMLElement* flowxml : xml->getChildren()) {
        unsigned int uniqueID = atoi(flowxml->getAttribute("id"));
        std::pair<unsigned int, unsigned int> item1 = {uniqueID, 0};
        dictFlowReceivedPktNum.insert(item1);
        std::pair<unsigned int, unsigned int> item2 = {uniqueID, 0};
        dictFlowSentPktNum.insert(item2);
        std::pair<unsigned int, std::vector<unsigned int>> item3 = {uniqueID, std::vector<unsigned int>(2, 0)};
        result.insert(item3);
    }
}

} /* namespace nesting */
