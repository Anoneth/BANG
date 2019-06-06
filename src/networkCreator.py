import sys
import random

size = -1
networkName = ""
iniFileName = ""
packageName = ""
parsim = False
nodeCount = -1

def printHelp():
    print("--size\t\t\t\tnetwork size, positive number")
    print("--network-name\t\t\tname of network, non-empty string")
    print("--package-name\t\t\tname of package (name of project), non-empty string")
    print("--ini-filename\t\t\tname of ini file, non-empty string")
    print("--parsim\t\t\tenable parsim, true/false [optional]")
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
        if sys.argv[i].startswith("--parsim="):
            try:
                parsim = bool(sys.argv[i][9:])
            except:
                print("wrong parsim value!")
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

if parsim and nodeCount < 0:
    print("parsim enable but node count non positive!")
    printHelp()

if nodeCount > 0 and not parsim:
    print("node count non-zero but parsim disable!")
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

connections = [[0 for y in range(size)] for x in range(size)]

conn = []

outFile = open(networkName + ".ned", "w")
outFile.write("package " + packageName + ";\n\n")
if not parsim:
    outFile.write("import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;\n")
    outFile.write("import inet.node.ethernet.Eth100M;\n")
    outFile.write("import inet.node.inet.Router;\n")
    outFile.write("import inet.node.inet.StandardHost;\n")
else:
    outFile.write("import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;\n")
    outFile.write("import inet.nodes.ethernet.Eth100M;\n")
    outFile.write("import inet.nodes.inet.Router;\n")
    outFile.write("import inet.nodes.inet.StandardHost;\n")
outFile.write("\n\n")
outFile.write("network " + networkName)
outFile.write("\n{\n")
outFile.write("\tsubmodules:\n")
outFile.write("\t\tconfigurator: Ipv4NetworkConfigurator;\n")

def createConnection(one, two):
    conn.append(one + ".ethg++ <--> Eth100M <--> " + two + ".ethg++")


def countConnections():
    count = 0
    for i in connections:
        for j in i:
            if j == 1:
                count += 1
    return count

def printModules(modules):
    for i in modules:
        if "router" in i:
            outFile.write("\t\t" + i + ": Router;\n")
        else:
            outFile.write("\t\t" + i + ": StandardHost;\n")


def printConnections():
    outFile.write("\tconnections:\n")
    for i in range(len(conn)):
        outFile.write("\t\t" + conn[i] + ";\n")


connections[0][1] = 1
connections[1][0] = 1

mods = []
i = 2
while i < size:
    isConnected = 0
    j = 0
    for j in range(i):
        tmp = 0
        for k in range(i):
            if (connections[j][k] == 1):
                tmp += 1

        c = random.uniform(0, 1)
        p = float(tmp) / countConnections()
        if c < p:
            isConnected = 1
            connections[j][i] = 1
            connections[i][j] = 1
    if isConnected == 0:
        i -= 1
    i += 1


for i in range(size):
    name = ""
    tmp = 0
    for j in range(size):
        if connections[i][j] == 1:
            tmp += 1
    if tmp > 1:
        name = "router"
        name += str(i)
        mods.append(name)
    else:
        name = "host"
        name += str(i)
        mods.append(name)

for i in range(size):
    for j in range(i, size):
        if connections[i][j] == 1:
            createConnection(mods[i], mods[j])
            
printModules(mods)

printConnections()
outFile.write("}")

outFile.close()

iniFile = open(iniFileName, "w")
iniFile.write("[General]\n")
iniFile.write("network = " + packageName + "." + networkName)
iniFile.write("\n\n")

if parsim:
    iniFile.write("parallel-simulation = true\n")
    iniFile.write("parsim-communications-class = \"cMPICommunications\"\n")
    iniFile.write("parsim-synchronization-class = \"cNullMessageProtocol\"\n\n")
    for i in reversed(range(nodeCount)):
        iniFile.write("**node" + str(i) + "*.ipv4.configurator.networkConfiguratorModule = \"configurator[" + str(i) + "]\n")

for i in reversed(range(nodeCount)):
    iniFile.write("**node" + str(i) + "*.partition-id = " + str(i) + "\n")

for i in reversed(range(nodeCount)):
    iniFile.write("**configurator[" + str(i) + "].partition-id = " +str(i) + "\n")

iniFile.write("**host*.numApps = 1\n")
iniFile.write("**host*.app[0].typename = \"UdpBasicApp\"\n")
iniFile.write("**host*.app[0].localPort = 32344\n")
iniFile.write("**host*.app[0].messageLength = 1000B\n")
iniFile.write("**host*.app[0].stopTime = 10s\n")
iniFile.write("**host*.app[0].destAddresses = moduleListByPath(\"**host*\")\n")
iniFile.write("**host*.app[0].destPort = 32344\n")
iniFile.write("**host*.app[0].sendInterval = 0.4s\n")

print("Complete!")