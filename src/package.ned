package mymodel;

import inet.networklayer.configurator.ipv4.Ipv4NodeConfigurator;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.EtherHost;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import ned.DatarateChannel;


@license(LGPL);
//
// TODO documentation
//
simple MyModule
{
    gates:
        input in[];
        output out[];
}

network MyNetwork
{
    parameters:
        int countOfNodes = default(2);
        int countOfSwitches = default(10);
    submodules:
        configurator: Ipv4NetworkConfigurator;
        myModules[countOfNodes]: MyModule;
    connections:
        for i=1..countOfNodes - 1 {
            myModules[0].out++ --> { delay = 100ms; } --> myModules[i].in++;
            myModules[0].in++ <-- { delay = 100ms; } <-- myModules[i].out++;
        }
}

