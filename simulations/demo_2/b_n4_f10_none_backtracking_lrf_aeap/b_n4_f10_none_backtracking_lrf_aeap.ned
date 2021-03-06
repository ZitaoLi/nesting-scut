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

package nesting.simulations.demo_2.b_n4_f10_none_backtracking_lrf_aeap;

import ned.DatarateChannel;
import nesting.node.ethernet.VlanEtherSwitchPreemptable2;
import nesting.node.ethernet.TSNHost;
import nesting.node.ethernet.FlowStatisticsApp;


network TestScenario
{
    types:
        channel C extends DatarateChannel
        {
            delay = 100.0ns;
            datarate = 1.0Gbps;
            per = 0.0;
        }
    submodules:
        flowStatisticsApp: FlowStatisticsApp {
            @display("p=589,398");
        }
        switch2: VlanEtherSwitchPreemptable2 {
            @display("p=345,144");
            gates:
                ethg[3];
        }
        switch3: VlanEtherSwitchPreemptable2 {
            @display("p=221,209");
            gates:
                ethg[4];
        }
        switch4: VlanEtherSwitchPreemptable2 {
            @display("p=436,235");
            gates:
                ethg[3];
        }
        switch5: VlanEtherSwitchPreemptable2 {
            @display("p=306,326");
            gates:
                ethg[4];
        }
        host1: TSNHost {
            @display("p=390,62");
        }
        host8: TSNHost {
            @display("p=87,203");
        }
        host6: TSNHost {
            @display("p=175,387");
        }
        host7: TSNHost {

            @display("p=414,398");
        }
    connections:
        host1.ethg$o --> C --> switch2.ethg$i[0];
        switch2.ethg$o[0] --> C --> host1.ethg$i;

        host8.ethg$o --> C --> switch3.ethg$i[3];
        switch3.ethg$o[3] --> C --> host8.ethg$i;

        host6.ethg$o --> C --> switch5.ethg$i[2];
        switch5.ethg$o[2] --> C --> host6.ethg$i;

        host7.ethg$o --> C --> switch5.ethg$i[3];
        switch5.ethg$o[3] --> C --> host7.ethg$i;

        switch2.ethg$o[1] --> C --> switch3.ethg$i[0];

        switch2.ethg$o[2] --> C --> switch4.ethg$i[0];

        switch3.ethg$o[0] --> C --> switch2.ethg$i[1];

        switch3.ethg$o[1] --> C --> switch4.ethg$i[1];

        switch3.ethg$o[2] --> C --> switch5.ethg$i[0];

        switch4.ethg$o[0] --> C --> switch2.ethg$i[2];

        switch4.ethg$o[1] --> C --> switch3.ethg$i[1];

        switch4.ethg$o[2] --> C --> switch5.ethg$i[1];

        switch5.ethg$o[0] --> C --> switch3.ethg$i[2];

        switch5.ethg$o[1] --> C --> switch4.ethg$i[2];

}
