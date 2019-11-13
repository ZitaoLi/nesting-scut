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

#ifndef NESTING_NODE_ETHERNET_STATISTICSAPP_H_
#define NESTING_NODE_ETHERNET_STATISTICSAPP_H_

#include <omnetpp.h>
#include <unordered_map>
#include <iostream>
#include "inet/common/packet/Packet.h"

using namespace omnetpp;

namespace nesting {

class StatisticsApp: public omnetpp::cSimpleModule {

private:
    std::unordered_map<unsigned int, unsigned int> dictFlowPort;
    std::unordered_map<unsigned int, float> dictFlowPER;

public:
    StatisticsApp();
    virtual ~StatisticsApp();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

private:
    virtual void loadXml();
    virtual void processPacketFromLowerLevel(inet::Packet *packet);
};

} /* namespace nesting */

#endif /* NESTING_NODE_ETHERNET_STATISTICSAPP_H_ */
