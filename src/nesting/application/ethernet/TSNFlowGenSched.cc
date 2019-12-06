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

#include "TSNFlowGenSched.h"

namespace nesting {

Define_Module(TSNFlowGenSched);

void TSNFlowGenSched::loadScheduleOrDefault(cXMLElement* xml) {
    std::string hostName =
            this->getModuleByPath(par("hostModule"))->getFullName();

    // earse [*] part
    std::string target = hostName;
    std::string _hostName;
    for (auto it = target.begin(); it != target.end(); it++) {
        if (*it != '[') {
            _hostName += *it;
        } else if (*it == '[') {
            break;
        }
    }
    hostName = _hostName;

    HostSchedule<Ieee8021QCtrl>* schedule;
    bool realScheduleFound = false;
    //try to extract the part of the schedule belonging to this host
    for (cXMLElement* hostxml : xml->getChildren()) {
        if (strcmp(hostxml->getTagName(), "cycle") != 0
                && hostxml->getAttribute("name") == hostName) {
            schedule = HostScheduleBuilder::createHostScheduleFromXML(hostxml,
                    xml);

            EV_DEBUG << getFullPath() << ": Found schedule for name "
                            << hostName << endl;

            realScheduleFound = true;
            break;
        }
    }
    //load empty schedule if there is no part that affects this host in the schedule xml
    if (!realScheduleFound) {
        cXMLElement* defaultXml = par("emptySchedule").xmlValue();
        schedule = HostScheduleBuilder::createHostScheduleFromXML(defaultXml,
                xml);
    }
    std::unique_ptr < HostSchedule < Ieee8021QCtrl >> schedulePtr(schedule);

    nextSchedule.reset();
    nextSchedule = move(schedulePtr);

}

} // namespace nesting
