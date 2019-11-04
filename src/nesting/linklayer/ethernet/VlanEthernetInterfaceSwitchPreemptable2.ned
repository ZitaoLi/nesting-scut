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

package nesting.linklayer.ethernet;

@namespace();

import nesting.ieee8021q.queue.BaseRecoveryFunction;
import nesting.ieee8021q.queue.Queuing;
import nesting.linklayer.ethernet.VLANEncap;
import nesting.linklayer.framePreemption.FrameForward;
import nesting.linklayer.framePreemption.EtherMACFullDuplexPreemptable;
import inet.linklayer.contract.IEthernetInterface;
import inet.linklayer.ethernet.IEtherEncap;
import inet.networklayer.common.InterfaceEntry;

import inet.linklayer.ethernet.EtherEncap;

//
// This module implements an ethernet interface that supports frame preemption.
//
module VlanEthernetInterfaceSwitchPreemptable2 extends InterfaceEntry like IEthernetInterface
{
    parameters:
        @class(inet::InterfaceEntry);
        string interfaceTableModule;
        string address = default("auto"); // MAC address
        string fcsMode @enum("declared","computed") = default("declared");
        @display("i=block/ifcard;bgl=2");
        *.interfaceTableModule = default(absPath(interfaceTableModule));
        *.fcsMode = fcsMode;
        bool framePreemptionDisabled = default(true);
    gates:
        input upperLayerIn;
        output upperLayerOut;
        inout phys @labels(EtherFrame);
    submodules:
        // mac supporting frame preemption
        mac: EtherMACFullDuplexPreemptable {
            @display("p=337,590");
        }
        queuing: Queuing {
            parameters:
                @display("p=424,289;q=l2queue");
        }
        vlanEncapE: VLANEncap {
            @display("p=252,397");
        }
        //        etherEncapE: <default("EtherEncap")> like IEtherEncap {
        //            parameters:
        //                @display("p=89,436");
        //        }
        etherEncapE: EtherEncap {
            parameters:
                @display("p=253,497");
        }
        vlanEncapP: VLANEncap {
            @display("p=424,397");
        }
        //        etherEncapP: <default("EtherEncap")> like IEtherEncap {
        //            parameters:
        //                @display("p=195,436");
        //        }
        etherEncapP: EtherEncap {
            parameters:
                @display("p=425,497");
        }
        frameForward1: FrameForward {
            @display("p=252,289");
        }
        baseRecoveryFunction: BaseRecoveryFunction {
            @display("p=584,92");
        }
        seqDecodeFunction: SeqDecodeFunction {
            @display("p=102,161");
        }
        seqEncodeFunction: SeqEncodeFunction {
            @display("p=584,161");
        }
        lowerStreamIentificationUp: LowerStreamIentification {
            @display("p=253,198");
        }
        frameForward2: FrameForward {
            @display("p=253,53");
        }
        lowerStreamIentificationDown: LowerStreamIentification {
            @display("p=424,53");
        }
        frameForward3: FrameForward {
            @display("p=424,198");
        }
    connections:
        vlanEncapE.upperLayerOut --> frameForward1.lowerLayerInE;
        vlanEncapP.upperLayerOut --> frameForward1.lowerLayerInP;


        queuing.eOut --> vlanEncapE.upperLayerIn;
        queuing.pOut --> vlanEncapP.upperLayerIn;

        etherEncapE.lowerLayerOut --> mac.upperLayerIn;
        mac.upperLayerOut --> etherEncapE.lowerLayerIn;

        etherEncapP.lowerLayerOut --> mac.upperLayerPreemptableIn;
        mac.upperLayerPreemptableOut --> etherEncapP.lowerLayerIn;

        vlanEncapE.lowerLayerOut --> etherEncapE.upperLayerIn;
        etherEncapE.upperLayerOut --> vlanEncapE.lowerLayerIn;

        vlanEncapP.lowerLayerOut --> etherEncapP.upperLayerIn;
        etherEncapP.upperLayerOut --> vlanEncapP.lowerLayerIn;

        mac.phys <--> { @display("m=s"); } <--> phys;
        baseRecoveryFunction.out --> seqEncodeFunction.in;
        frameForward1.upperLayerOut --> lowerStreamIentificationUp.in;
        lowerStreamIentificationUp.outFRER --> seqDecodeFunction.in;
        seqDecodeFunction.out --> frameForward2.lowerLayerInE;
        lowerStreamIentificationUp.outNonFRER --> frameForward2.lowerLayerInP;
        frameForward2.upperLayerOut --> upperLayerOut;
        upperLayerIn --> lowerStreamIentificationDown.in;
        lowerStreamIentificationDown.outFRER --> baseRecoveryFunction.in;
        lowerStreamIentificationDown.outNonFRER --> frameForward3.lowerLayerInE;
        seqEncodeFunction.out --> frameForward3.lowerLayerInP;
        frameForward3.upperLayerOut --> queuing.in;
}
