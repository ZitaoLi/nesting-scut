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

simple SeqCodeFunction
{
    parameters:
        @display("i=block/square");
        @class(SeqCodeFunction);
        bool tagUntaggedFrames = default(false);	// If true, the encap process will add a VLAN Tag to untagged frames. This alters the frame size.
//        int pvid = default(1);						// Default port-VID inserted in control info of (vlan-)untagged frames. 
        bool registerProtocol = default(false);
//        @signal[encapPk](type=cPacket);
//        @signal[decapPk](type=cPacket);
//        @statistic[encapPk](title="packets encapsulated"; source=encapPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
//        @statistic[decapPk](title="packets decapsulated"; source=decapPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        bool verbose = default(false);
    gates:
        input in;
        output out;
}

simple SeqEncodeFunction extends SeqCodeFunction
{
    @class(SeqEncodeFunction);
}

simple SeqDecodeFunction extends SeqCodeFunction
{
    @class(SeqDecodeFunction);
}