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

package nesting.simulations.examples;

import ned.DatarateChannel;
import nesting.node.ethernet.VlanEtherSwitchPreemptable2;
import nesting.node.ethernet.VlanEtherHostSched2;
import nesting.node.ethernet.VlanEtherHostFullLoad;
import nesting.node.ethernet.VlanEtherHostQ2;


network TestScenario6
{
    types:
        channel C extends DatarateChannel
        {
            delay = 0.1us;
            datarate = 1Gbps;
        }
    submodules:
        switchA: VlanEtherSwitchPreemptable2 {
            parameters:
                @display("p=188,127");
            gates:
                ethg[5];
        }
        switchB: VlanEtherSwitchPreemptable2 {
            parameters:
                @display("p=327,127");
            gates:
                ethg[3];
        }
        switchC: VlanEtherSwitchPreemptable2 {
            parameters:
                @display("p=266,197");
            gates:
                ethg[2];
        }
        robotController: VlanEtherHostSched2 {
            @display("p=79,202");
        }
        roboticArm: VlanEtherHostQ2 {
            @display("p=440,202");
        }
        workstation1: VlanEtherHostFullLoad {
            @display("p=79,68");
        }
        workstation2: VlanEtherHostFullLoad {
            @display("p=188,29");
        }
    connections:
        robotController.ethg <--> C <--> switchA.ethg[0];
        roboticArm.ethg <--> C <--> switchB.ethg[2];
        switchA.ethg[1] <--> C <--> switchB.ethg[0];

        switchA.ethg[2] <--> C <--> switchC.ethg[0];

        switchC.ethg[1] <--> C <--> switchB.ethg[1];

        workstation1.ethg <--> C <--> switchA.ethg[3];
        workstation2.ethg <--> C <--> switchA.ethg[4];
}
