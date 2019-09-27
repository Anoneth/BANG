package mymodel;

<<<<<<< HEAD
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.Router;


@license(LGPL);
//
// TODO documentation
//

network BANGNetwork
{
    parameters:
        int countOfNodes = default(1);
        int countOfRouters = default(10);
        int hostsPerRouter = default(10);
        int rndSeed = default(2122);
    submodules:
        //configurator[countOfNodes]: Ipv4NetworkConfigurator;
        myModules[countOfNodes]: MyModule;
    connections:
        for i=1..countOfNodes - 1 {
            BANGModules[0].out++ --> {  delay = 100ms; } --> BANGModules[i].in++;
            BANGModules[0].in++ <-- {  delay = 100ms; } <-- BANGModules[i].out++;
        }
=======
simple MyModule
{

>>>>>>> 63eb855788ede925bf9a408230acdcbc798e9d98
}

