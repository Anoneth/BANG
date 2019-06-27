import sys
import random

size = -1
networkName = ""
iniFileName = ""
packageName = ""
parsim = False
nodeCount = -1

treeLans = []

def printHelp():
    print("--size\t\t\t\tnetwork size, positive number")
    print("--network-name\t\t\tname of network, non-empty string")
    print("--package-name\t\t\tname of package (name of project), non-empty string")
    print("--ini-filename\t\t\tname of ini file, non-empty string")
    print("--node-count\t\t\tcount of nodes in parsim, positive number [optional]")
    exit()

if len(sys.argv) > 1:
    for i in range(1, len(sys.argv)):
        if sys.argv[i].startswith("--help"):
            printHelp()
        if sys.argv[i] == "-h":
            printHelp()
        if sys.argv[i].startswith("--size="):
            try:
                size = int(sys.argv[i][7:])
                if size < 0:
                    raise Exception()
            except:
                print("wrong size!")
                printHelp()
        if sys.argv[i].startswith("--node-count="):
            try:
                nodeCount = int(sys.argv[i][13:])
                if nodeCount < 0:
                    raise Exception()
            except:
                print("wrong node count!")
                printHelp()
        if sys.argv[i].startswith("--network-name="):
            networkName = sys.argv[i][15:]
            if len(networkName) < 1:
                print("empty network name!")
                printHelp()
        if sys.argv[i].startswith("--package-name="):
            packageName = sys.argv[i][15:]
            if len(packageName) < 1:
                print("empty package name!")
                printHelp()
        if sys.argv[i].startswith("--ini-filename="):
            iniFileName = sys.argv[i][15:]
            if len(iniFileName) < 1:
                print("empty ini file name")
                printHelp()

else:
    printHelp()

if size < 0:
    print("size must be postive value!")
    printHelp()

if len(networkName) < 1:
    print("no network name specified!")
    printHelp()

if len(packageName) < 1:
    print("empty package name!")
    printHelp()

if len(iniFileName) < 1:
    print("empty ini file name")
    printHelp()

if nodeCount > 0:
    parsim = True

connections = [[0 for y in range(size)] for x in range(size)]

conn = []

tt = []

outFile = open(networkName + ".ned", "w")
outFile.write("package " + packageName + ";\n\n")
if not parsim:
    outFile.write("import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;\n")
    outFile.write("import inet.node.ethernet.Eth100M;\n")
    outFile.write("import inet.node.inet.Router;\n")
    outFile.write("import inet.node.inet.StandardHost;\n")
else:
    outFile.write("import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;\n")
    outFile.write("import inet.nodes.ethernet.Eth100M;\n")
    outFile.write("import inet.nodes.ethernet.EtherSwitch;\n")
    outFile.write("import inet.nodes.inet.Router;\n")
    outFile.write("import inet.nodes.inet.StandardHost;\n")
outFile.write("\n\n")
outFile.write("network " + networkName)
outFile.write("\n{\n")
if not parsim:
    outFile.write("\tparameters:\n")
    outFile.write("\t\tint size = default(3);\n")
outFile.write("\tsubmodules:\n")
if not parsim:
    outFile.write("\t\tconfigurator: Ipv4NetworkConfigurator;\n")
else:
    outFile.write("\t\tconfigurator[" + str(nodeCount) + "]: IPv4NetworkConfigurator;\n")

def createConnection(one, two):
    t1 = ".ethg++"
    if "treeLan" in one:
        t1 = ".ethg"
    t2 = ".ethg++"
    if "treeLan" in two:
        t2 = ".ethg"
    conn.append(one + t1 + " <--> Eth100M <--> " + two + t2)


def countConnections():
    count = 0
    for i in connections:
        for j in i:
            count += j
    return count

def printModules(modules):
    for i in modules:
        if "router" in i:
            outFile.write("\t\t" + i + ": Router;\n")
        else:
            if parsim:
                outFile.write("\t\t" + i + ": TreeNetwork;\n")
            else:
                outFile.write("\t\t" + i + ": StandardHost;\n")


def printConnections():
    outFile.write("\tconnections:\n")
    for i in range(len(conn)):
        outFile.write("\t\t" + conn[i] + ";\n")

count = 0

if parsim:
    for i in range(size):
        tt.append(0)

    connections[0][1] = 1
    connections[1][0] = 1

    tt[0] = 1
    tt[1] = 1

    count = 2
    mods = []
    i = 2
    while i < size:
        isConnected = False
        j = 0
        for j in range(i):

            c = random.uniform(0, 1)
            p = float(tt[j]) / count
            if c < p:
                isConnected = True
                connections[j][i] = 1
                connections[i][j] = 1
                count += 1
                tt[i] += 1
                tt[j] += 1
        if not isConnected:
            i -= 1
        i += 1


    for i in range(size):
        name = ""
        if tt[i] > 1:
            name = "router"
            name += str(i)
            mods.append(name)
        else:
            name = "treeLan"
            name += str(i)
            treeLans.append(name)
            mods.append(name)

    for i in range(size):
        for j in range(i, size):
            if connections[i][j] == 1:
                createConnection(mods[i], mods[j])

    printModules(mods)

    printConnections()
else:
    outFile.write("\t\tmyModule: MyModule;\n")
outFile.write("}\n")

if parsim:
    part = open("part.txt", "r")
    t = part.read()
    outFile.write(t)

iniFile = open(iniFileName, "w")
iniFile.write("[General]\n")
iniFile.write("network = " + packageName + "." + networkName)
iniFile.write("\n\n")

if parsim:
    iniFile.write("parallel-simulation = true\n")
    iniFile.write("parsim-communications-class = \"cMPICommunications\"\n")
    iniFile.write("parsim-synchronization-class = \"cNullMessageProtocol\"\n\n")
    iniFile.write("**router**.networkLayer.configurator.networkConfiguratorModule = \"configurator[0]\"\n")
    for i in reversed(range(len(treeLans))):
        iniFile.write("**" + treeLans[i] + "**.networkLayer.configurator.networkConfiguratorModule = \"configurator[" + str(i % nodeCount) + "]\"\n")
    iniFile.write("\n**router**.partition-id = 0\n\n")
    for i in reversed(range(len(treeLans))):
        iniFile.write("**" + treeLans[i] + "**.partition-id = " + str(i % nodeCount) + "\n")
    iniFile.write("\n**.**.mac.address = \"auto\"\n")
    iniFile.write("**.configurator[*].config = xmldoc(\"routes.xml\")\n")
    iniFile.write("**.configurator[*].addStaticRoutes = true\n")
    iniFile.write("**.configurator[*].optimizeRoutes = false\n")
    iniFile.write("**.configurator[*].storeHostAddresses = true\n\n")
    iniFile.write("**.numUdpApps = 1\n**.udpApp[0].typename=\"UDPBackboneApp\"\n**.udpApp[*].destPort = 1\n**.udpApp[*].localPort = 1\n**.udpApp[*].messageLength = exponential(200B)\n**.udpApp[*].sendInterval = exponential(2.162162162162162e-05s)\n**.udpApp[*].startTime = 0\n**.udpApp[*].probabilitySendLocal = 0.0\n")
    iniFile.write("**.h = 3\n")
    iniFile.write("**.k = 3\n")
    iniFile.write("**.numLeafHosts = 5\n")
    iniFile.write("**.numInnerHosts = 3\n")
else:
    iniFile.write("**.size = " + str(size) + "\n")


for i in reversed(range(nodeCount)):
    iniFile.write("**configurator[" + str(i) + "].partition-id = " +str(i) + "\n")

if not parsim:
    iniFile.write("**host*.numApps = 1\n")
    iniFile.write("**host*.app[0].typename = \"UdpBasicApp\"\n")
    iniFile.write("**host*.app[0].localPort = 32344\n")
    iniFile.write("**host*.app[0].messageLength = 1000B\n")
    iniFile.write("**host*.app[0].stopTime = 10s\n")
    iniFile.write("**host*.app[0].destAddresses = moduleListByPath(\"**host*\")\n")
    iniFile.write("**host*.app[0].destPort = 32344\n")
    iniFile.write("**host*.app[0].sendInterval = 0.4s\n")

out = open(networkName + ".txt", "w")

for i in range(size):
    st = ""
    for j in range(size):
        st += str(connections[i][j]) + ","
    st += "\n"
    out.write(st)

if parsim:
    routes = open("routes.xml", "w")
    routes.write("<config>\n")
    routes.write("\t<interface hosts='*.router*' address='10.x.x.x' netmask='255.x.x.x'/>\n")
    for i in range(len(treeLans)):
        routes.write("\t<interface hosts='*" + treeLans[i] + ".**' address='" + str(11 + i) + ".x.x.x' netmask='255.x.x.x'/>\n")
    routes.write("</config>")

print("Complete!")
