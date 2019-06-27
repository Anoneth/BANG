#include <string.h>
#include <vector>
#include <omnetpp.h>

using namespace omnetpp;

class BANGModule: public cSimpleModule {
public:
<<<<<<< HEAD:src/bangmodule.h
    BANGModule();
    ~BANGModule();
=======
    MyModule();
    ~MyModule();
>>>>>>> 63eb855788ede925bf9a408230acdcbc798e9d98:src/mymodule.h
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

<<<<<<< HEAD:src/bangmodule.h
=======
    std::vector<int> *counts;

>>>>>>> 63eb855788ede925bf9a408230acdcbc798e9d98:src/mymodule.h
    unsigned int gateIt;
    unsigned int countOfNodes;
    unsigned int countOfRouters;
    unsigned int hostsPerRouter;
    unsigned int tmp;

<<<<<<< HEAD:src/bangmodule.h
    std::vector<std::vector<int>*> *connections;
    std::vector<unsigned int> *countConnections;
=======
    bool **connections;
>>>>>>> 63eb855788ede925bf9a408230acdcbc798e9d98:src/mymodule.h

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
