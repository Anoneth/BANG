import sys
import random
import networkx as nx
import numpy as np

size = -1
networkName = ""
iniFileName = ""
packageName = ""
parsim = False
nodeCount = -1
seed = -1

hosts = []
traffic_pairs = []
edge_nodes = []
hosts_ip = []



def printHelp():
    print("--size\t\t\t\tnetwork size, positive number")
    print("--network-name\t\t\tname of network, non-empty string")
    print("--package-name\t\t\tname of package (name of project), non-empty string")
    print("--ini-filename\t\t\tname of ini file, non-empty string")
    print("--node-count\t\t\tcount of nodes in parsim, positive number [optional]")
    print("--seed\t\t\t\trandom seed, positive number [optional]")
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
        if sys.argv[i].startswith("--seed="):
            try:
                seed = int(sys.argv[i][7:])
            except:
                print("wrong seed!")
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

if seed < 0:
    seed = random.randint(0, sys.maxsize)
rnd = random.Random(seed)

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
outFile.write("\tparameters:\n")
outFile.write("\t\tint nodeCount = default(2);\n")
if not parsim:
    outFile.write("\t\tint size = default(3);\n")
outFile.write("\tsubmodules:\n")
if not parsim:
    outFile.write("\t\tconfigurator: Ipv4NetworkConfigurator;\n")
else:
    outFile.write("\t\tconfigurator[nodeCount]: IPv4NetworkConfigurator;\n")

def createConnection(left, right): 
    conn.append(left + ".ethg++ <--> Eth100M <--> " + right + ".ethg++")


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
    
            c = rnd.uniform(0, 1)
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
            mods.append("edge_router" + str(i))
            edge_nodes.append(i)

    for i in range(len(edge_nodes) // 2):
        left = edge_nodes[rnd.randint(0, len(edge_nodes) // 2)]
        right = edge_nodes[rnd.randint(len(edge_nodes) // 2, len(edge_nodes) - 1)]
        traffic_pairs.append([left, right])

    for i in range(size):
        for j in range(i, size):
            if connections[i][j] == 1:
                createConnection(mods[i], mods[j])

    for i in edge_nodes:
        count = rnd.randint(1, 5)
        for j in range(count):
            name = "host" + str(i) + "_" + str(j)
            mods.append(name)
            hosts.append(name)
            createConnection(mods[i], name)
            ip = "10." + str(i % 255) + "." + str(j % 255) + ".1"
            hosts_ip.append([name, ip])
            
    printModules(mods)

    printConnections()
else:
    outFile.write("\t\tbang: BANG;\n")
outFile.write("}\n")

#if parsim:
#    part = open("part.txt", "r")
#    t = part.read()
#    outFile.write(t)

iniFile = open(iniFileName, "w")
iniFile.write("#File generated by networkCreator.py\n")
iniFile.write("#rnd-seed: " + str(seed) + "\n")
iniFile.write("[General]\n")
iniFile.write("network = " + packageName + "." + networkName)
iniFile.write("\n\n")

sequences = []
def make_seq_rec(seq, num, n):
    newSeq = seq.copy()
    if n > 0:
        newSeq.append(num)
    if n == len(traffic_pairs):
        flag = 0
        for i in seq:
            flag |= 1 << i
        if flag == 2 ** nodeCount - 1:
            sequences.append(seq)
    else:
        if n == len(traffic_pairs) - 1:
            make_seq_rec(newSeq, 0, n + 1)
        else:
            for i in range(nodeCount):
                make_seq_rec(newSeq, i, n + 1)

def make_seq_norec():
    seq = [0] * len(traffic_pairs)
    max_n = nodeCount ** len(traffic_pairs)
    for i in range(max_n):
        tmp = i
        for j in reversed(range(len(seq))):
            seq[j] = tmp % nodeCount
            tmp = tmp // nodeCount
        flag = 0
        for i in seq:
            flag |= 1 << i
        if flag == 2 ** nodeCount - 1:
            sequences.append(seq.copy())

if parsim:
    iniFile.write("parallel-simulation = true\n")
    iniFile.write("parsim-communications-class = \"cMPICommunications\"\n")
    iniFile.write("parsim-synchronization-class = \"cNoSynchronization\"\n\n")
    iniFile.write("\n**.**.mac.address = \"auto\"\n")
    
    iniFile.write("**.configurator[*].config = xmldoc(\"src/" + networkName + ".xml\")\n")
    iniFile.write("**.configurator[*].addStaticRoutes = true\n")
    iniFile.write("**.configurator[*].optimizeRoutes = false\n")
    iniFile.write("**.configurator[*].storeHostAddresses = true\n\n")
    iniFile.write("**.numUdpApps = 1\n**.udpApp[0].typename=\"UDPBackboneApp\"\n**.udpApp[*].destPort = 1\n**.udpApp[*].localPort = 1\n**.udpApp[*].messageLength = exponential(200B)\n**.udpApp[*].sendInterval = exponential(1e-03s)\n**.udpApp[*].startTime = 0\n**.udpApp[*].probabilitySendLocal = 0.0\n")

    addresses = [""] * size

    unique = set()
    for i in traffic_pairs:
        unique.add(i[0])
        unique.add(i[1])
        for j in hosts_ip:
            if j[0].startswith("host" + str(i[1]) + "_"):
                addresses[i[0]] += j[1] + " "
            if j[0].startswith("host" + str(i[0]) + "_"):
                addresses[i[1]] += j[1] + " "


    for i in unique:
        iniFile.write("**.host" + str(i) + "_**.udpApp[*].destAddresses = \"")
        iniFile.write(addresses[i])
        iniFile.write("\"\n")


    adj_matr = np.matrix(connections)
    graph = nx.from_numpy_matrix(adj_matr)
    pairs_on_router = [set() for i in range(size)]
    paths = []

    for pair in traffic_pairs:
        path = nx.shortest_path(graph, pair[0], pair[1])
        for i in path:
            pairs_on_router[i].add(pair[0])
            pairs_on_router[i].add(pair[1])
        paths.append(path)
    
    #make_seq_rec([], 0, 0)
    #make_seq_norec()

    #for i in sequences:
        

    traffic_groups = [set()]
    traffic_groups[0].add(traffic_pairs[0][0])
    traffic_groups[0].add(traffic_pairs[0][1])
    for i in range(1, len(traffic_pairs)):
        j = 0
        flag = True
        while j < len(traffic_groups) and flag:
            tmp = traffic_pairs[i]
            tmp2 = traffic_groups[j]
            if tmp[0] in tmp2 or tmp[1] in tmp2:
                traffic_groups[j].add(tmp[0])
                traffic_groups[j].add(tmp[1])
                flag = False
            j += 1
        if flag:
            tmp3 = set()
            tmp3.add(tmp[0])
            tmp3.add(tmp[1])
            traffic_groups.append(tmp3)
                                
        else:
            flag = True
            while flag:
                k = 0
                while k < len(traffic_groups):
                    l = k + 1
                    while l < len(traffic_groups):
                        m = 0
                        tmp3 = list(traffic_groups[k])
                        while m < len(tmp3):
                            if tmp3[m] in traffic_groups[l]:
                                for n in traffic_groups[l]:
                                    traffic_groups[k].add(n)
                                traffic_groups.pop(l)
                                break
                            m += 1
                        if m < len(traffic_groups[k]):
                            break
                        l += 1
                    if l < len(traffic_groups):
                        break
                    k += 1
                if k == len(traffic_groups):
                    flag = False
    print("Optimal node count: " + str(len(traffic_groups)))

    traffic_groups.sort(key=lambda x: len(x), reverse=True)

    partition = [[] for i in range(size)]
    for i in range(size):
        tmpset = set()
        for j in pairs_on_router[i]:
            for index, k in enumerate(traffic_groups):
                if j in k:
                    tmpset.add(index)
        partition[i] = list(tmpset)
        if len(partition[i]) == 0:
            partition[i].append(len(traffic_groups) - 1)


    host_groups = set()
    for i in hosts:
        host_groups.add(i.split("_")[0])
    host_groups_list = list(host_groups)
    
    
    iniFile.write(networkName + ".nodeCount=" + str(len(traffic_groups)) + "\n")
    for i in range(len(traffic_groups)):
        iniFile.write("**configurator[" + str(i) + "].partition-id = " + str(i) + "\n")
    
    for i in range(len(host_groups_list)):
        node = 0
        index = int(host_groups_list[i][4:])
        j = 0
        while j < len(traffic_groups):
            if index in traffic_groups[j]:
                node = j
                break
            j += 1
        iniFile.write("**" + host_groups_list[i] + "**.networkLayer.configurator.networkConfiguratorModule = \"configurator[" + str(node) + "]\"\n")
        iniFile.write("**" + host_groups_list[i] + "**.partition-id=" + str(node) + "\n\n")
        iniFile.write("**edge_router" + str(index) + "**.networkLayer.configurator.networkConfiguratorModule = \"configurator[" + str(node) + "]\"\n")
        iniFile.write("**edge_router" + str(index) + "**.partition-id=" + str(node) + "\n\n")

    for i, nodes in enumerate(partition):
        iniFile.write("\n**router" + str(i) + ".partition-id = " + str(nodes[0]) + "\n\n")
        iniFile.write("**router" + str(i) + ".networkLayer.configurator.networkConfiguratorModule = \"configurator[" + str(nodes[0]) + "]\"\n")
    
    routes = open(networkName + ".xml", "w")
    routes.write("<config>\n")
    for i in range(size):
        routes.write("\t<interface hosts='**router" + str(i) + 
                    "' address='11." + str(i // 256) + "." + str(i % 256) + ".x' netmask='255.255.255.x'/>\n")
    for i in hosts_ip:
        routes.write("\t<interface hosts='" + i[0] + 
                    "' address='" + i[1] + 
                    "' netmask='255.255.255.x'/>\n")
    routes.write("</config>")

else:
    iniFile.write("**.size = " + str(size) + "\n")
    iniFile.write("**host*.numApps = 1\n")
    iniFile.write("**host*.app[0].typename = \"UdpBasicApp\"\n")
    iniFile.write("**host*.app[0].localPort = 32344\n")
    iniFile.write("**host*.app[0].messageLength = 1000B\n")
    iniFile.write("**host*.app[0].stopTime = 10s\n")
    iniFile.write("**host*.app[0].destAddresses = moduleListByPath(\"**host*\")\n")
    iniFile.write("**host*.app[0].destPort = 32344\n")
    iniFile.write("**host*.app[0].sendInterval = 0.4s\n")

print("Complete!")
