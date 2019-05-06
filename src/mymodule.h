#include <string.h>
#include <vector>
#include <omnetpp.h>
#include <omnetpp/ccomponenttype.h>

#include "SimpleMessage_m.h"

using namespace omnetpp;

class MyModule: public cSimpleModule {
public:
    ~MyModule();
protected:
    virtual int numInitStages() const override { return 15; }
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

private:
    enum TYPE {
        ROUTER, HOST
    };
    std::vector<cModule*> routers;
    std::vector<cModule*> hosts;
    std::vector<cDatarateChannel*> channels;
    unsigned int j;
    unsigned int countOfNodes;
    unsigned int countOfRouters;
    unsigned int hostsPerRouter;
    unsigned int tmp;

    std::vector<MyPair*> *connectionPairs;
    std::vector<MyPair*> *hostCount;
    std::vector<unsigned int> *countConnections;

    void doStage0();
    void doStage1();
    void doStage2();

    bool isMaster();
    bool endWith(std::string string, std::string &end);

    cModule *createModule(TYPE type, const char *name);
    void addChannels(int count);
    void connectModules(cModule *f, cModule *s);

    void connectModules(unsigned int left, unsigned int right);

    void createModules();

    void handleMessageModule1(cMessage *msg);
    void handleMessageModule2(cMessage *msg);

    void sendBroadcast(cMessage *msg);

    unsigned int countOfConnections();

    cModule *findModule(std::string &name);

};
