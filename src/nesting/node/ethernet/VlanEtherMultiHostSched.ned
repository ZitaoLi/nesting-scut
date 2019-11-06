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

import ned.DatarateChannel;
import ned.IdealChannel;
import inet.linklayer.ethernet.EtherMacFullDuplex;
import inet.linklayer.ethernet.IEtherEncap;
import inet.networklayer.common.InterfaceTable;
import inet.linklayer.contract.IEthernetInterface;
import inet.linklayer.ethernet.EthernetInterface;
import inet.linklayer.contract.IEtherMac;
import inet.networklayer.common.InterfaceEntry;
import inet.linklayer.ethernet.EtherMacFullDuplex;
import nesting.node.ethernet.MergingMac;

channel CC extends DatarateChannel
{
    @display("m=n");
    delay = 0us;
    datarate = 100Gbps;  // this may cause transmission delay
}

//channel CC extends IdealChannel
//{
//    @display("m=n");
//}

//
// TODO auto-generated type
//
module VlanEtherMultiHostSched
{
    parameters:
        @display("i=device/pc2");
        @networkNode();
        @labels(node,ethernet-node);
        int numberOfFlow = default(1);
        string fcsMode @enum("declared","computed") = default("declared");
        *.interfaceTableModule = default(absPath(".interfaceTable"));
        *.fcsMode = fcsMode;
    gates:
        inout ethg @labels(EtherFrame-conn);
    submodules:
        flow1[numberOfFlow]: VlanEtherHostSched2 {
            @display("p=217,41,r,200");
        }
        mergingPipe: MergingPipe {
            @display("p=217,216;is=l");
        }
        toHost[numberOfFlow]: MergingMac {
            parameters:
                @display("p=217,122,r,200");
        }
        toChannel: MergingMac {
            @display("p=97,216");
        }
        //        toHost[numberOfFlow]: EthernetInterface {
        //            @display("p=82,123,row,150;q=txQueue");
        //        }
        //        toChannel: EthernetInterface {
        //            @display("p=82,277,row,150;q=txQueue");
        //        }
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=47,33;is=s");
        }
    connections:
        for i=0..numberOfFlow-1 {
            flow1[i].ethg <--> CC <--> toHost[i].phys;
            toHost[i].upperLayerOut --> { @display("m=n"); } --> mergingPipe.hostIn++;
            toHost[i].upperLayerIn <-- { @display("m=n"); } <-- mergingPipe.hostOut++;
        }
        mergingPipe.channelOut --> { @display("m=n"); } --> toChannel.upperLayerIn;
        mergingPipe.channelIn <-- { @display("m=n"); } <-- toChannel.upperLayerOut;
        toChannel.phys <--> { @display("m=n"); } <--> ethg;
}
