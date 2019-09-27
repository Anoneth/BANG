package bang;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth100M;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;

network BANGNetwork
{
	parameters:
		int size = default(3);
	submodules:
		configurator: Ipv4NetworkConfigurator;
		bang: BANGModule;
}
