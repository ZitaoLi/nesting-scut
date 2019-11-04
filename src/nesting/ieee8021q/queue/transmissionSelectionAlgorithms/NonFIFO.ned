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

package nesting.ieee8021q.queue.transmissionSelectionAlgorithms;

//
// TODO auto-generated module
//
simple NonFIFO like TSAlgorithm
{
    parameters:
        @display("i=block/server");
    	@class(NonFIFO);
    	string macModule; // Path to the fp module
        string gateModule; // Path to the transmission gate module
        string queueModule; // Path to the length-aware-queue module
		string gateControllerModule = "^.gateController"; // Path to the gate controller module
        bool verbose = default(false);
    gates:
        input in;
        output out;
}
