import sys
import random

countOfNodes = int(sys.argv[1])
countOfRouters = int(sys.argv[2])
hostPerRouter = int(sys.argv[3])
nedFileName = sys.argv[4]
packageName = sys.argv[5]
networkName = sys.argv[6]
iniFileName = sys.argv[7]

connections = []

outFile = open(sys.argv[4], "w")
outFile.write("package " + sys.argv[5] + ";\n\n")
outFile.write("import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;\n")
outFile.write("import inet.node.ethernet.Eth100M;\n")
outFile.write("import inet.node.inet.Router;\n")
outFile.write("import inet.node.inet.StandardHost;\n")
outFile.write("\n\n")
outFile.write("network " + sys.argv[6])
outFile.write("\n{\n")
outFile.write("\tsubmodules:\n")
outFile.write("\t\tconfigurator: Ipv4NetworkConfigurator;\n")

def createConnection(one, two):
    connections.append(one + ".ethg++ <--> Eth100M <--> " + two + ".ethg++")


def countConnections(module):
    count = 0
    for i in range(len(connections)):
        if module in connections[i]:
            count += 1
    return count

def printRouters(routers):
    for i in range(len(routers)):
        outFile.write("\t\t" + routers[i] + ": Router;\n")


def printHosts(hosts):
    for i in range(len(hosts)):
        outFile.write("\t\t" + hosts[i] + ": StandardHost;\n")


def printConnections():
    outFile.write("\tconnections:\n")
    for i in range(len(connections)):
        outFile.write("\t\t" + connections[i] + ";\n")


routers = []
routers.append("node" + str(1 % countOfNodes) + "router" + str(1))
routers.append("node" + str(0 % countOfNodes) + "router" + str(0))
createConnection(routers[0], routers[1])

for i in range(2, countOfRouters):
    name = "node" + str(i % countOfNodes) + "router" + str(i)
    routers.append(name)
    j = 0
    notConnected = True
    while notConnected and j < len(routers):
        rnd = random.uniform(0, 1)
        p = float(countConnections(routers[j])) / (2 * len(connections))
        if (rnd < p):
            createConnection(name, routers[j])
            notConnected = False
        j += 1
    if (j == len(routers)):
        createConnection(name, routers[0])

hosts = []

for i in range(len(routers)):
    if (countConnections(routers[i]) == 1):
        for j in range(hostPerRouter):
            name = "node" + str(i % countOfNodes) + "router" + str(i) + "host" + str(j)
            hosts.append(name)
            createConnection(name, routers[i])

printRouters(routers)
printHosts(hosts)
printConnections()
outFile.write("}")

outFile.close()

iniFile = open(sys.argv[7], "w")
iniFile.write("[General]\n")
iniFile.write("network = " + sys.argv[5] + "." + sys.argv[6])
iniFile.write("\n\n")

iniFile.write("parallel-simulation = true\n")
iniFile.write("parsim-communications-class = \"cMPICommunications\"\n")
iniFile.write("parsim-synchronization-class = \"cNullMessageProtocol\"\n\n")

iniFile.write("**configurator.partition-id = 0\n")

for i in range(countOfNodes):
    iniFile.write("**node" + str(i) + "*.partition-id = " + str(i) + "\n")

