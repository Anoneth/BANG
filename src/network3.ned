package mymodel;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth100M;
import inet.node.inet.Router;


@license(LGPL);
//
// TODO documentation
//

network TestNetwork
{
    submodules:
        configurator[countOfNodes]: Ipv4NetworkConfigurator;
        router: Router;
        router1: Router;
    connections:
        router1.ethg++ <--> Eth100M <--> router.ethg++;
}
