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

@namespace();

import ned.DatarateChannel;
import inet.linklayer.ethernet.EtherMacFullDuplex;
import inet.linklayer.ethernet.IEtherEncap;
import inet.networklayer.common.InterfaceTable;
import inet.linklayer.contract.IEthernetInterface;
import inet.linklayer.ethernet.EthernetInterface;
import inet.linklayer.contract.IEtherMac;
import inet.networklayer.common.InterfaceEntry;
import inet.linklayer.ethernet.EtherMacFullDuplex;

//
// TODO auto-generated type
//
module MergingMac extends InterfaceEntry like IEthernetInterface
{
    parameters:
        @class(inet::InterfaceEntry);
        string interfaceTableModule;
        string address = default("auto"); // MAC address
        string fcsMode @enum("declared","computed") = default("declared");
        @display("i=block/ifcard;bgl=2");
        *.interfaceTableModule = default(absPath(interfaceTableModule));
        *.fcsMode = fcsMode;
    gates:
        inout phys @labels(EtherFrame);
        input upperLayerIn;
        output upperLayerOut;
    submodules:
        mac: EtherMacFullDuplex {
            @display("p=82,128");
        }
//        interfaceTable: InterfaceTable {
//            parameters:
//                @display("p=182,128;is=s");
//        }
    connections:
        mac.phys <--> { @display("m=s"); } <--> phys;
        mac.upperLayerIn <-- { @display("m=n"); } <-- upperLayerIn;
        mac.upperLayerOut --> { @display("m=n"); } --> upperLayerOut;
}
