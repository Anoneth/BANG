#include <string.h>
#include <vector>
#include <omnetpp.h>

using namespace omnetpp;

class MyModule: public cSimpleModule {
public:
    MyModule();
    ~MyModule();
protected:
    virtual int numInitStages() const override { return 15; }
    virtual void initialize(int stage) override;
    virtual void finish() override;

private:
    enum TYPE {
        ROUTER, HOST, CONFIGURATOR
    };

    std::vector<cModule*> configurators;
    std::vector<cModule*> *mods;

    std::vector<cDatarateChannel*> channels;

    std::vector<int> *counts;

    unsigned int gateIt;
    unsigned int countOfNodes;
    unsigned int countOfRouters;
    unsigned int hostsPerRouter;
    unsigned int tmp;

    bool **connections;

    void doStage0();
    void doStage1();
    void doStage2();
    void doStage3();

    cModule *createModule(TYPE type, const char *name);
    void addChannels(int count);
    void connectModules(cModule *f, cModule *s);

    void createModules();

    unsigned int countOfConnections();

};
