[TOC]
## Configuruation file

This file descripes about how to right the configuration files.

### Whole configuration directory structure

The configuration file is located under `xml` directory.

```
xml
├── routes
│   └── [scenario name]
│       └── routes_flat.xml
└── schedules
    └── TestScenario8
        ├── hosts
        │   ├── hosts_flat.xml
        │   └── [host name]
        │       └──  flow[flow-id].xml
        └── switches
            ├── GCLs_flat.xml
            └── [switch name]

```

### How to write them?

Write the following into `.ini` file which you want to run.

#### Configure routes

+ Switch configuration

```
**.filteringDatabase.database = xmldoc("xml/routes/[scenario name]/routes_flat.xml", "/filteringDatabases/")
**.filteringDatabase.cycle = xmldoc("xml/schedules/TestScenario8/switches/GCLs_flat.xml", "/schedule/")
```

#### Configure schedules

+ Switch configuration

```
**.gateController.initialSchedule = xmldoc("xml/schedules/[scenario name]/switches/GCLs_flat.xml")
```

or

```
**.[switch name].eth[*].queuing.gateController.initialSchedule = xmldoc("xml/schedules/[scenario name]/switches/[switch name]/GCL.xml")
```

+ Host configuration

```
**.[host name].flow[0].trafGenSchedApp.initialSchedule = xmldoc("xml/schedules/[scenario name]/hosts/[host name]/flow[flow-id].xml")
```

### Example of each .xml file

+  `routes_flat.xml`

```
<?xml version="1.0" ?>
<filteringDatabases>
	<filteringDatabase id="[switch name]">
	    <static>
	        <forward>
	        	<multicastAddress macAddress="[multicast mac address]" ports="1 2" /> <!-- support many ports -->
	        	<individualAddress macAddress="[unicast mac address]" ports="1" /> <!-- only single port -->
	        </forward>
	    </static>
	</filteringDatabase>
</filteringDatabases>
```

+ `GCLs_flat.xml` and `GCL.xml`

```
<?xml version="1.0" ?>
<schedule>
	<!-- hyper period -->
	<cycle><!-- us -->[cycle time]</cycle>
	<!-- switch -->
	<switch name="[switch name]">
		<!-- GCL-->
		<port id="[port id]">
			<!-- GCL item -->
			<entry>
				<length><!-- us -->[time length]</length>
				<bitvector><!-- bit arrary -->[gate states]</bitvector>
				<uniqueID><!-- int -->[flow-id]</uniqueID>
				<phase><!-- int -->[phase]</phase>
			</entry>
		</port>
	</switch>
</schedule>

```

+ `flow[flow-id].xml`

```
<?xml version="1.0" ?>
<schedule>
	<cycle><!-- us -->[time length]</cycle>
	<!-- flow-id=1 -->
	<host name="flow"> <!-- the name "flow" is fixed -->
		<!-- schedule entry -->
		<entry>
			<start><!-- us -->[start time]</start>
			<queue>7</queue>
			<dest>[mac address]</dest> <!-- the mac address can be multicast or unicast -->
			<group>[mac address]</group> <!-- the mac address must be multicast 
			<size><!-- byte -->[packet size]</size> <!-- overhead is not included -->
			<uniqueID><!-- int -->[flow-id]</uniqueID>
			<phase><!-- int -->[phase]</phase>
		</entry>
	</host>
</schedule>
```