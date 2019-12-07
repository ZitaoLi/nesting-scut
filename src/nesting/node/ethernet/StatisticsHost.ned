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

package nesting.node.ethernet;

import inet.linklayer.ethernet.EthernetInterface;
import inet.networklayer.common.InterfaceTable;
import nesting.linklayer.ethernet.VLANEncap;
import nesting.linklayer.ethernet.VLANTagREncap;
import nesting.node.ethernet.StatisticsApp;

//
// TODO auto-generated type
//
module StatisticsHost
{
    parameters:
        string fcsMode @enum("declared","computed") = default("declared");
        @networkNode();
        @labels(node,ethernet-node);
        @display("i=device/pc2;bgb=385,371");
        *.interfaceTableModule = default(absPath(".interfaceTable"));
        *.fcsMode = fcsMode;
    gates:
        inout ethg @labels(EtherFrame-conn);
    submodules:
        vlanEncap: VLANEncap {
            @display("p=188,233");
        }
        vlanTagREncap: VLANTagREncap {
            @display("p=188,147");
        }
        eth: EthernetInterface {
            parameters:
                @display("p=188,309,row,150;q=txQueue");
        }
        statisticsApp: StatisticsApp {
            @display("p=188,61");
        }
        interfaceTable: InterfaceTable {
            @display("p=70,61;is=s");
        }
    connections:
        ethg <--> { @display("n=m"); } <--> eth.phys;

        eth.upperLayerOut --> { @display("n=m"); } --> vlanEncap.lowerLayerIn;
        eth.upperLayerIn <-- { @display("n=m"); } <-- vlanEncap.lowerLayerOut;

        vlanEncap.upperLayerOut --> { @display("n=m"); } --> vlanTagREncap.lowerLayerIn;
        vlanEncap.upperLayerIn <-- { @display("n=m"); } <-- vlanTagREncap.lowerLayerOut;

//        statisticsApp.port$o --> { @display("n=m"); } --> vlanTagREncap.upperLayerIn;
//        statisticsApp.port$i <-- { @display("n=m"); } <-- vlanTagREncap.upperLayerOut;
		statisticsApp.fromLowerLayer$o --> { @display("n=m"); } --> vlanTagREncap.upperLayerIn;
		statisticsApp.fromLowerLayer$i <-- { @display("n=m"); } <-- vlanTagREncap.upperLayerOut;
}
