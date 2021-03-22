package src;

import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.node.ethernet.Eth100M;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;

network BANGNetwork
{
	parameters:
		int size = default(3);
	submodules:
		configurator: IPv4NetworkConfigurator;
		bang: BANGModule;
}
