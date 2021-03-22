package src;

import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.node.inet.Router;


@license(LGPL);
//
// TODO documentation
//
simple BANGModule
{
    gates:
        input in[];
        output out[];
}

network BANGNetwork1
{
    parameters:
        int countOfNodes = default(1);
        int countOfRouters = default(10);
        int hostsPerRouter = default(10);
        int rndSeed = default(2122);
    submodules:
        //configurator[countOfNodes]: IPv4NetworkConfigurator;
        BANGModules[countOfNodes]: BANGModule;
    connections:
        for i=1..countOfNodes - 1 {
            BANGModules[0].out++ --> {  delay = 100ms; } --> BANGModules[i].in++;
            BANGModules[0].in++ <-- {  delay = 100ms; } <-- BANGModules[i].out++;
        }
}


