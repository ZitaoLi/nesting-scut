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

import inet.linklayer.ethernet.EtherMacFullDuplex;
import inet.linklayer.ethernet.IEtherEncap;
import inet.linklayer.ethernet.EthernetInterface;
import inet.networklayer.common.InterfaceTable;
import inet.linklayer.contract.IEthernetInterface;
import nesting.application.ethernet.TSNFlowGenSched;
import nesting.ieee8021q.clock.IClock;
import nesting.ieee8021q.queue.gating.ScheduleSwap;
import nesting.linklayer.ethernet.VLANEncap;
import nesting.linklayer.ethernet.VLANTagREncap;
import nesting.node.ethernet.StatisticsApp;

//
// TODO auto-generated type
//
module TSNHost
{
    parameters:
        int numberOfFlow = default(1);
        string fcsMode @enum("declared","computed") = default("declared");
        @networkNode();
        @labels(node,ethernet-node);
        @display("i=device/pc2;bgb=500,500");
        *.interfaceTableModule = default(absPath(".interfaceTable"));
        *.fcsMode = fcsMode;
    gates:
        inout ethg @labels(EtherFrame-conn);
    submodules:
        flows[numberOfFlow]: TSNFlowGenSched {
            @display("p=321,61,r,200");
        }
        statisticsApp: StatisticsApp {
            @display("p=188,61");
        }
        vlanTagREncap: VLANTagREncap {
            @display("p=188,243");
        }
        vlanEncap: VLANEncap {
            @display("p=188,327");
        }
        eth: EthernetInterface {
            @display("p=188,413,row,150;q=txQueue");
        }
        clock: <default("IdealClock")> like IClock {
            @display("p=65,61");
        }
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=65,157;is=s");
        }
        mergingPipe: MergingPipe {
            @display("p=188,157;is=l");
        }
    connections:
        statisticsApp.port <--> mergingPipe.toStatistics;
        for i=0..numberOfFlow-1 {
        	flows[i].in  <-- mergingPipe.hostOut++;
        	flows[i].out --> mergingPipe.hostIn++;
        }
        mergingPipe.channelIn <-- vlanTagREncap.upperLayerOut;
        mergingPipe.channelOut --> vlanTagREncap.upperLayerIn;
        vlanTagREncap.lowerLayerIn <-- vlanEncap.upperLayerOut;
        vlanTagREncap.lowerLayerOut --> vlanEncap.upperLayerIn;
        vlanEncap.lowerLayerIn <-- eth.upperLayerOut;
        vlanEncap.lowerLayerOut --> eth.upperLayerIn;
        eth.phys <--> ethg;
}
