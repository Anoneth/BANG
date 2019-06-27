package mymodel;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth100M;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;


network TestParallel
{
	parameters:
		int size = default(3);
	submodules:
		configurator: Ipv4NetworkConfigurator;
		myModule: MyModule;
}
