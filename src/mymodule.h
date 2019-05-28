#include <string.h>
#include <vector>
#include <omnetpp.h>
#include <omnetpp/ccomponenttype.h>

#include "MyPair.h"

using namespace omnetpp;

class MyModule: public cSimpleModule {
public:
    ~MyModule();
protected:
    virtual int numInitStages() const override { return 15; }
    virtual void initialize(int stage) override;
    virtual void finish() override;

private:
    enum TYPE {
        ROUTER, HOST
    };
    std::vector<cModule*> routers;
    std::vector<cModule*> hosts;
    std::vector<cDatarateChannel*> channels;
    unsigned int gateIt;
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
    void doStage3();

    cModule *createModule(TYPE type, const char *name);
    void addChannels(int count);
    void connectModules(cModule *f, cModule *s);

    void connectModules(unsigned int left, unsigned int right);

    void createModules();

    unsigned int countOfConnections();

    cModule *findModule(std::string &name);

};
