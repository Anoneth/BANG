#include "mymodule.h"

Define_Module(MyModule);

MyModule::~MyModule() {
    finish();
}

void MyModule::finish() {
    cGate *gate;
    for (unsigned int i = 0; i < channels.size(); i++) {
        gate = channels[i]->getSourceGate();
        if (gate != nullptr)
            gate->disconnect();
        else
            delete channels[i];
    }
}

bool MyModule::isMaster() {
    std::string name = std::string(this->getFullName());
    std::string end = std::string("[0]");
    return name.compare(name.size() - end.size(), end.size(), end) == 0;
}

bool MyModule::endWith(std::string string, std::string &end) {
    return string.compare(string.size() - end.size(), end.size(), end) == 0;
}

void MyModule::initialize(int stage) {
    switch (stage) {
    case 0:
        doStage0();
        break;
    case 13:
        doStage1();
        break;
    case 14:
        doStage2();
    default:
        break;
    }
}

void MyModule::doStage0() {
    channels = std::vector<cDatarateChannel*>();
    routers = std::vector<cModule*>();
    hosts = std::vector<cModule*>();

    countOfNodes = getSimulation()->getModule(1)->par("countOfNodes").intValue()
            - 1;
    countOfSwitches =
            getSimulation()->getModule(1)->par("countOfSwitches").intValue();
    tmp = countOfNodes;

    addChannels(2 * (countOfSwitches - 1));
    if (getSimulation()->getActiveEnvir()->getParsimNumPartitions() != 1
            || isMaster()) {
        createModules();
    }
}

void MyModule::doStage1() {
    if (getSimulation()->getActiveEnvir()->getParsimNumPartitions() != 1
            || isMaster()) {
        j = 0;
        for (unsigned int i = 0; i < countOfSwitches; i++) {
            std::string name = "switchNode";
            name.append(std::to_string(i % (countOfNodes + 1)));
            name.append("[" + std::to_string(i) + "]");
            createModule(ROUTER, name.c_str());
        }
        MyPair *pair;
        for (unsigned int i = 0; i < connectionPairs->size(); i++) {
            pair = connectionPairs->at(i);
            connectModules(routers[pair->left], routers[pair->right]);
        }
        hostCount = new std::vector<MyPair*>();
        for (unsigned int i = 0; i < countOfSwitches; i++) {
            if (countConnections->at(i) == 1) {
                int count = rand() % 10;
                hostCount->push_back(new MyPair(i, count));
                addChannels(2 * count);
            }
        }
    }
}

void MyModule::doStage2() {
    if (getSimulation()->getActiveEnvir()->getParsimNumPartitions() != 1
            || isMaster()) {
        for (unsigned int i = 0; i < hostCount->size(); i++) {
            std::string baseName = "hostNode";
            baseName.append(
                    std::to_string(hostCount->at(i)->left % (countOfNodes + 1)).append(
                            "["));
            for (int j = 0; j < hostCount->at(i)->right; j++) {
                std::string name = baseName + std::to_string(j).append("]");
                cModule *host = createModule(HOST, name.c_str());
                connectModules(
                        findModule(
                                std::string("[").append(std::to_string(hostCount->at(i)->left)).append(
                                        "]")), host);
            }
        }
    }
}

void MyModule::addChannels(int count) {
    cChannelType *channelType = cChannelType::get("inet.node.ethernet.Eth100M");
    unsigned int countPrev = channels.size();
    for (unsigned int i = countPrev; i < countPrev + count; i++) {
        channels.push_back(channelType->createDatarateChannel("channel"));
        channels[i]->setDatarate(1e08);
        channels[i]->setDelay(5e-08);
        channels[i]->setBitErrorRate(0);
        channels[i]->setPacketErrorRate(0);
    }
}

void MyModule::handleMessage(cMessage *msg) {
    if (isMaster()) {
        handleMessageModule1(msg);
    } else {
        handleMessageModule2(msg);
    }
    delete msg;
}

void MyModule::handleMessageModule1(cMessage *msg) {
    if (strcmp("Stage 1", msg->getName()) == 0) {
        hostCount = new std::vector<MyPair*>();
        SimpleMessage *smsg = new SimpleMessage("Stage 2");
        for (unsigned int i = 0; i < connectionPairs->size(); i++) {
            if (connectionPairs->at(i)->right == 0) {
                int count = (int) (uniform(0, 1) * 10) % 10;
                hostCount->push_back(new MyPair(i, count));
                addChannels(2 * count);
            }
        }
        smsg->setPairsArraySize(hostCount->size());
        for (unsigned int i = 0; i < hostCount->size(); i++) {
            smsg->setPairs(i, *(hostCount->at(i)));
        }
        sendBroadcast((cMessage*) smsg);
    } else if (strcmp("Complete", msg->getName()) == 0) {
        cMessage *msg = new cMessage("Complete");
        sendBroadcast(msg);
    }
}

void MyModule::handleMessageModule2(cMessage *msg) {
    if (strcmp(msg->getName(), "Stage 1") == 0) {
        SimpleMessage *smsg = (SimpleMessage*) msg;
        unsigned int count = smsg->getNamesArraySize();
        for (unsigned int i = 0; i < count; i++) {
            createModule(ROUTER, smsg->getNames(i));
        }
        for (unsigned int i = 0; i < smsg->getPairsArraySize(); i++) {
            MyPair pair = smsg->getPairs(i);
            std::string name = std::string("[") + std::to_string(pair.left)
                    + std::string("]");
            cModule *left = findModule(name);
            name = std::string("[") + std::to_string(pair.right)
                    + std::string("]");
            cModule *right = findModule(name);
            connectModules(left, right);
        }
        send(new cMessage("Stage 1"), this->gate("out", 0));
    } else if (strcmp(msg->getName(), "Stage 2") == 0) {
        SimpleMessage *smsg = (SimpleMessage*) msg;
        unsigned int count = smsg->getPairsArraySize();
        hostCount = new std::vector<MyPair*>(count);
        for (unsigned int i = 0; i < count; i++) {
            MyPair pair = smsg->getPairs(i);
            hostCount->at(i) = new MyPair(pair.left, pair.right);
        }
        addChannels(2 * count);
        send(new cMessage("Stage 2"), this->gate("out", 0));
    } else if (strcmp(msg->getName(), "Complete") == 0) {
        send(new cMessage("Complete"), this->gate("out", 0));
    }
}

void MyModule::connectModules(unsigned int left, unsigned int right) {
    connectionPairs->push_back(new MyPair(left, right));
    countConnections->at(left)++;countConnections
    ->at(right)++;}

void MyModule::createModules() {
    unsigned int gateCount = 0;
    connectionPairs = new std::vector<MyPair*>();
    countConnections = new std::vector<unsigned int>();
    gateCount += 2;
    countConnections->push_back(1);
    countConnections->push_back(1);
    connectionPairs->push_back(new MyPair(1, 0));
    for (unsigned int i = 2; i < countOfSwitches; i++) {
        unsigned int count = countConnections->size();
        countConnections->push_back(0);
        bool notConnected = false;
        unsigned int j = 0;
        while (!notConnected && j < count) {
            double c = uniform(0, 1);
            double p = (double) countConnections->at(j)
                    / countOfConnections();
            if (c < p) {
                notConnected = true;
                connectModules(i, j);
                gateCount += 2;
            }
            j++;
        }
        if (j == count)
            connectModules(i, 0);
    }
}

cModule* MyModule::createModule(TYPE type, const char *name) {
    cModule *mod;
    cModuleType *moduleType;
    switch (type) {
    case ROUTER:
        moduleType = cModuleType::get("inet.node.inet.Router");
        break;
    case HOST:
        moduleType = cModuleType::get("inet.node.inet.StandardHost");
        break;
    default:
        moduleType = nullptr;
        break;
    }

    mod = moduleType->create(std::string(name).c_str(),
            getSimulation()->getModule(1));

    switch (type) {
    case ROUTER:
        routers.push_back(mod);
        break;
    case HOST:
        hosts.push_back(mod);
        break;
    default:
        break;
    }

    mod->finalizeParameters();
    mod->setGateSize("ethg", 1);
    mod->buildInside();
    mod->scheduleStart(simTime());
    return mod;
}

void MyModule::connectModules(cModule *f, cModule *s) {
    int fc = f->gateSize("ethg");
    int sc = s->gateSize("ethg");
    f->setGateSize("ethg", fc + 1);
    s->setGateSize("ethg", sc + 1);
    f->gate("ethg$o", fc - 1)->connectTo(s->gate("ethg$i", sc - 1),
            channels[j++]);
    s->gate("ethg$o", sc - 1)->connectTo(f->gate("ethg$i", fc - 1),
            channels[j++]);
}

unsigned int MyModule::countOfConnections() {
    unsigned int res = 0;
    for (unsigned int i : *countConnections) {
        res += i;
    }
    return res;
}

cModule* MyModule::findModule(std::string &name) {
    for (unsigned int i = 0; i < routers.size(); i++)
        if (endWith(routers[i]->getFullName(), name))
            return routers[i];
    return 0;
}

void MyModule::sendBroadcast(cMessage *msg) {
    for (int i = 0; i < this->gateSize("out"); i++)
        send(msg->dup(), this->gate("out", i));
    delete msg;
}
