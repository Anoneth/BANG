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
    channels.clear();
}

void MyModule::initialize(int stage) {
    switch (stage) {
    case 14:
        doStage0();
        doStage1();
        doStage2();
        doStage3();
        break;
    case 15:
        doStage3();
        break;
    case 17:
        doStage2();
        break;
    default:
        break;
    }
}

void MyModule::doStage0() {
    channels = std::vector<cDatarateChannel*>();
    routers = std::vector<cModule*>();
    hosts = std::vector<cModule*>();

    countOfNodes =
            getSimulation()->getModule(1)->par("countOfNodes").intValue();

    countOfRouters =
            getSimulation()->getModule(1)->par("countOfRouters").intValue();
    tmp = countOfNodes;

    hostsPerRouter =
            getSimulation()->getModule(1)->par("hostsPerRouter").intValue();
    createModules();
}

void MyModule::doStage1() {
    gateIt = 0;
    for (unsigned int i = 0; i < countOfRouters; i++) {
        std::string name = "router";
        name.append(std::to_string(i));
        name.append(std::string("node"));
        name.append(std::to_string(i % countOfNodes));
        createModule(ROUTER, name.c_str());
    }
    MyPair *pair;
    for (unsigned int i = 0; i < connectionPairs->size(); i++) {
        pair = connectionPairs->at(i);
        connectModules(routers[pair->left], routers[pair->right]);
    }
    hostCount = new std::vector<MyPair*>();
    for (unsigned int i = 0; i < countOfRouters; i++) {
        //if (countConnections->at(i) == 1) {
        int count = hostsPerRouter;
        hostCount->push_back(new MyPair(i, count));
        //}
    }
}

void MyModule::doStage2() {
    int n = 0;
    for (unsigned int i = 0; i < hostCount->size(); i++) {
        std::string baseName = "router";
        MyPair *pair = hostCount->at(i);
        baseName.append(std::to_string(pair->left));
        baseName.append(std::string("node"));
        baseName.append(std::to_string(pair->left % countOfNodes));
        for (int j = 0; j < pair->right; j++) {
            std::string name = baseName + std::string("host")
                    + std::to_string(n++);
            cModule *host = createModule(HOST, name.c_str());
            connectModules(findModule(baseName), host);
        }
    }
}

void MyModule::doStage3() {
    for (int t = 0; t < 15; t++) {
        for (auto i : routers) {
            i->callInitialize(t);
        }

        for (auto i : hosts) {
            i->callInitialize(t);
        }

        for (auto i : channels) {
            i->callInitialize(t);
        }
    }
}

void MyModule::addChannels(int count) {
    cChannelType *channelType = cChannelType::get("inet.node.ethernet.Eth100M");
    for (int i = 0; i < count; i++) {
        cDatarateChannel *ch = channelType->createDatarateChannel("channel");
        ch->setDatarate(1e08);
        ch->setDelay(5e-08);
        ch->setBitErrorRate(0);
        ch->setPacketErrorRate(0);
        channels.push_back(ch);
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
    for (unsigned int i = 2; i < countOfRouters; i++) {
        unsigned int count = countConnections->size();
        countConnections->push_back(0);
        unsigned int j = 0;
        while (j < count) {
            double c = uniform(0, 1);
            double p = (double) countConnections->at(j) / countOfConnections();
            if (c < p) {
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

    mod = moduleType->create(name, getSimulation()->getModule(1));

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
    return mod;
}

void MyModule::connectModules(cModule *f, cModule *s) {
    addChannels(2);

    int fc = f->gateSize("ethg");
    int sc = s->gateSize("ethg");

    f->setGateSize("ethg", fc + 1);
    s->setGateSize("ethg", sc + 1);

    cChannel *ch1 = channels[gateIt++];
    cChannel *ch2 = channels[gateIt++];

    f->gate("ethg$o", fc - 1)->connectTo(s->gate("ethg$i", sc - 1), ch1);
    s->gate("ethg$o", sc - 1)->connectTo(f->gate("ethg$i", fc - 1), ch2);
}

unsigned int MyModule::countOfConnections() {
    unsigned int res = 0;
    for (unsigned int i : *countConnections) {
        res += i;
    }
    return res;
}

cModule* MyModule::findModule(std::string &name) {
    for (unsigned int i = 0; i < routers.size(); i++) {
        if (strcmp(routers[i]->getFullName(), name.c_str()) == 0)
            return routers[i];
    }
    return 0;
}
