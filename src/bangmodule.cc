#include "bangmodule.h"

Define_Module(BANGModule);

BANGModule::BANGModule() {
    channels = std::vector<cDatarateChannel*>();
    mods = new std::vector<cModule*>();

    configurators = std::vector<cModule*>();

    countOfRouters =
            getSimulation()->getModule(1)->par("size").longValue();
    tmp = countOfNodes;

    counts = new std::vector<int>(countOfRouters, 0);

    connections = new bool*[countOfRouters];
    for (int i = 0; i < countOfRouters; i++) {
        connections[i] = new bool[countOfRouters];
        for (int j = 0; j < countOfRouters; j++)
            connections[i][j] = false;
    }
}

BANGModule::~BANGModule() {
    finish();
}

void BANGModule::finish() {
    cGate *gate;
    for (unsigned int i = 0; i < channels.size(); i++) {
        gate = channels[i]->getSourceGate();
        if (gate != nullptr)
            gate->disconnect();
        else
            delete channels[i];
    }
    channels.clear();
    delete connections;
}

void BANGModule::initialize(int stage) {
    switch (stage) {
    case 14:
        doStage0();
        doStage1();
        doStage2();
        doStage3();
        break;
    default:
        break;
    }
}

void BANGModule::doStage0() {
    createModules();
}

void BANGModule::doStage1() {
    gateIt = 0;

    for (int i = 0; i < countOfRouters; i++) {
        std::string name;
        if (counts->at(i) > 1) {
            name = "router";
            name.append(std::to_string(i));
            createModule(ROUTER, name.c_str());
        } else {
            name = "host";
            name.append(std::to_string(i));
            createModule(HOST, name.c_str());
        }
    }
}

void BANGModule::doStage2() {
    for (int i = 0; i < countOfRouters; i++)
        for (int j = i; j < countOfRouters; j++)
            if (connections[i][j])
                connectModules(mods->at(i), mods->at(j));
}

void BANGModule::doStage3() {
    for (int t = 0; t < 15; t++) {

        for (auto i : configurators) {
            i->callInitialize(t);
        }

        for (auto i : channels) {
            i->callInitialize(t);
        }

        for (auto i : *mods) {
            i->callInitialize(t);
        }
    }
}

void BANGModule::addChannels(int count) {
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

void BANGModule::createModules() {
    connections[0][1] = true;
    connections[1][0] = true;
    int count = 2;
    counts->at(0) = 1;
    counts->at(1) = 1;
    for (int i = 2; i < countOfRouters; i++) {
        bool isConnected = false;
        int j = 0;
        for (j = 0; j < i; j++) {
            double c = uniform(0, 1);
            double p = (double) counts->at(j) / count;
            if (c < p) {
                isConnected = true;
                connections[j][i] = true;
                connections[i][j] = true;
                counts->at(i) += 1;
                counts->at(j) += 1;
                count++;
            }
        }
        if (!isConnected) {
            i--;
        }
    }
}

cModule* BANGModule::createModule(TYPE type, const char *name) {
    cModule *mod;
    cModuleType *moduleType;
    switch (type) {
    case ROUTER:
        moduleType = cModuleType::get("inet.node.inet.Router");
        break;
    case HOST:
        moduleType = cModuleType::get("inet.node.inet.StandardHost");
        break;
    case CONFIGURATOR:
        moduleType = cModuleType::get(
                "inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator");
        break;
    default:
        moduleType = nullptr;
        break;
    }

    mod = moduleType->create(name, getSimulation()->getModule(1));

    mod->finalizeParameters();

    switch (type) {
    case ROUTER:
        //routers.push_back(mod);
        mod->setGateSize("ethg", 1);
        mods->push_back(mod);
        break;
    case HOST:
        //hosts.push_back(mod);
        mod->setGateSize("ethg", 1);
        mods->push_back(mod);
        break;
    case CONFIGURATOR:
        configurators.push_back(mod);
        break;
    default:
        break;
    }

    mod->buildInside();
    return mod;
}

void BANGModule::connectModules(cModule *f, cModule *s) {
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

unsigned int BANGModule::countOfConnections() {
    unsigned int res = 0;
    for (int i = 0; i < countOfRouters; i++)
        for (int j = 0; j < countOfRouters; j++)
            if (connections[i][j])
                res++;
    return res;
}
