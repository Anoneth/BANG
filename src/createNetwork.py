import random

countOfNodes = 4
countOfRouters = 20
hostPerRouter = 5

connections = []

outFile = open("simpleNetwork.ned", "w")
outFile.write("package mymodel;\n\n")
outFile.write("import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;\n")
outFile.write("import inet.node.ethernet.Eth100M;\n")
outFile.write("import inet.node.inet.Router;\n")
outFile.write("import inet.node.inet.StandardHost;\n")
outFile.write("\n\n")
outFile.write("network SimpleNetwork\n")
outFile.write("{\n")
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
routers.append("node" + str(0 % countOfNodes) + "router" + str(0))
routers.append("node" + str(1 % countOfNodes) + "router" + str(1))
createConnection(routers[0], routers[1])

for i in range(2, countOfRouters):
    name = "node" + str(i % countOfNodes) + "router" + str(i)
    routers.append(name)
    j = 0
    notConnected = True
    while notConnected and j < len(routers):
        rnd = random.uniform(0, 1)
        p = countConnections(routers[j]) / (2 * len(connections))
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

