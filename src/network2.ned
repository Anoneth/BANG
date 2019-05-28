package mymodel;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;


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
        int countOfNodes = default(1);
        int countOfRouters = default(10);
        int hostsPerRouter = default(10);
        int rndSeed = default(2122);
    submodules:
        configurator[countOfNodes]: Ipv4NetworkConfigurator;
        myModules[countOfNodes]: MyModule;
    connections:
        for i=1..countOfNodes - 1 {
            myModules[0].out++ --> { delay = 100ms; } --> myModules[i].in++;
            myModules[0].in++ <-- { delay = 100ms; } <-- myModules[i].out++;
        }
}

