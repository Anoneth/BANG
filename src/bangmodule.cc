#include "bangmodule.h"

Define_Module(BANGModule);

BANGModule::BANGModule() {
    channels = std::vector<cDatarateChannel*>();
    connections = new std::vector<std::vector<int>*>();
    mods = new std::vector<cModule*>();

    configurators = std::vector<cModule*>();

    countOfNodes =
            getSimulation()->getModule(1)->par("countOfNodes").intValue();

    countOfRouters =
            getSimulation()->getModule(1)->par("countOfRouters").intValue();
    tmp = countOfNodes;

    hostsPerRouter =
            getSimulation()->getModule(1)->par("hostsPerRouter").intValue();
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
    int c = getSimulation()->getActiveEnvir()->getParsimNumPartitions();
    if (c == 0) {
        createModule(CONFIGURATOR, "configurator");
    }
    for (int i = 0; i < c; i++) {
        std::string name = "configurator";
        name.append(
                std::to_string(
                        getSimulation()->getActiveEnvir()->getParsimProcId()));
        createModule(CONFIGURATOR, name.c_str());
    }
}

void BANGModule::doStage1() {
    gateIt = 0;
    for (unsigned int i = 0; i < connections->size(); i++) {
        std::string name;
        if (connections->at(i)->size() > 1) {
            if (getSimulation()->getActiveEnvir()->getParsimNumPartitions()
                    > 1) {
                name = "node";
                name.append(std::to_string(i % countOfNodes));
            } else
                name = "router";
            name.append(std::to_string(i));
            createModule(ROUTER, name.c_str());
        } else {
            if (getSimulation()->getActiveEnvir()->getParsimNumPartitions()
                    > 1) {
                name = "node";
                name.append(std::to_string(i % countOfNodes));
            } else
                name = "host";
            name.append(std::to_string(i));
            createModule(HOST, name.c_str());
        }
    }
}

void BANGModule::doStage2() {
    for (unsigned int i = 0; i < countOfRouters; i++) {
        for (unsigned int j = 0; j < connections->at(i)->size(); j++) {
            int right = connections->at(i)->at(j);
            //printf("from %i\tto %i\n", i, right);
            if (i < right)
                connectModules(mods->at(i), mods->at(right));
        }
    }
}

void BANGModule::doStage3() {
    for (int t = 0; t < 15; t++) {
        //configurator->callInitialize(t);
        /*for (auto i : routers) {
         i->callInitialize(t);
         }

         for (auto i : hosts) {
         i->callInitialize(t);
         }*/

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
    connections->push_back(new std::vector<int>());
    connections->push_back(new std::vector<int>());
    connections->at(0)->push_back(1);
    connections->at(1)->push_back(0);
    for (unsigned int i = 2; i < countOfRouters; i++) {
        connections->push_back(new std::vector<int>());
        bool isConnected = false;
        for (unsigned int j = 0; (j < connections->size() - 1) && !isConnected;
                j++) {
            double c = uniform(0, 1);
            double p = (double) connections->at(j)->size()
                    / countOfConnections();
            if (c < p) {
                //isConnected = true;
                connections->at(j)->push_back(i);
                connections->at(i)->push_back(j);
            }
        }
        if (!isConnected) {
            i--;
            connections->pop_back();
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
    for (auto i : *connections) {
        res += i->size();
    }
    return res;
}
