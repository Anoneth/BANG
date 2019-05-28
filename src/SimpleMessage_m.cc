//
// Generated file, do not edit! Created by nedtool 5.4 from SimpleMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "SimpleMessage_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(SimpleMessage)

SimpleMessage::SimpleMessage(const char *name, short kind) : ::omnetpp::cMessage(name,kind)
{
    pairs_arraysize = 0;
    this->pairs = 0;
    names_arraysize = 0;
    this->names = 0;
}

SimpleMessage::SimpleMessage(const SimpleMessage& other) : ::omnetpp::cMessage(other)
{
    pairs_arraysize = 0;
    this->pairs = 0;
    names_arraysize = 0;
    this->names = 0;
    copy(other);
}

SimpleMessage::~SimpleMessage()
{
    delete [] this->pairs;
    delete [] this->names;
}

SimpleMessage& SimpleMessage::operator=(const SimpleMessage& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void SimpleMessage::copy(const SimpleMessage& other)
{
    delete [] this->pairs;
    this->pairs = (other.pairs_arraysize==0) ? nullptr : new MyPair[other.pairs_arraysize];
    pairs_arraysize = other.pairs_arraysize;
    for (unsigned int i=0; i<pairs_arraysize; i++)
        this->pairs[i] = other.pairs[i];
    delete [] this->names;
    this->names = (other.names_arraysize==0) ? nullptr : new ::omnetpp::opp_string[other.names_arraysize];
    names_arraysize = other.names_arraysize;
    for (unsigned int i=0; i<names_arraysize; i++)
        this->names[i] = other.names[i];
}

void SimpleMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    b->pack(pairs_arraysize);
    doParsimArrayPacking(b,this->pairs,pairs_arraysize);
    b->pack(names_arraysize);
    doParsimArrayPacking(b,this->names,names_arraysize);
}

void SimpleMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    delete [] this->pairs;
    b->unpack(pairs_arraysize);
    if (pairs_arraysize==0) {
        this->pairs = 0;
    } else {
        this->pairs = new MyPair[pairs_arraysize];
        doParsimArrayUnpacking(b,this->pairs,pairs_arraysize);
    }
    delete [] this->names;
    b->unpack(names_arraysize);
    if (names_arraysize==0) {
        this->names = 0;
    } else {
        this->names = new ::omnetpp::opp_string[names_arraysize];
        doParsimArrayUnpacking(b,this->names,names_arraysize);
    }
}

void SimpleMessage::setPairsArraySize(unsigned int size)
{
    MyPair *pairs2 = (size==0) ? nullptr : new MyPair[size];
    unsigned int sz = pairs_arraysize < size ? pairs_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        pairs2[i] = this->pairs[i];
    pairs_arraysize = size;
    delete [] this->pairs;
    this->pairs = pairs2;
}

unsigned int SimpleMessage::getPairsArraySize() const
{
    return pairs_arraysize;
}

MyPair& SimpleMessage::getPairs(unsigned int k)
{
    if (k>=pairs_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", pairs_arraysize, k);
    return this->pairs[k];
}

void SimpleMessage::setPairs(unsigned int k, const MyPair& pairs)
{
    if (k>=pairs_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", pairs_arraysize, k);
    this->pairs[k] = pairs;
}

void SimpleMessage::setNamesArraySize(unsigned int size)
{
    ::omnetpp::opp_string *names2 = (size==0) ? nullptr : new ::omnetpp::opp_string[size];
    unsigned int sz = names_arraysize < size ? names_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        names2[i] = this->names[i];
    for (unsigned int i=sz; i<size; i++)
        names2[i] = 0;
    names_arraysize = size;
    delete [] this->names;
    this->names = names2;
}

unsigned int SimpleMessage::getNamesArraySize() const
{
    return names_arraysize;
}

const char * SimpleMessage::getNames(unsigned int k) const
{
    if (k>=names_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", names_arraysize, k);
    return this->names[k].c_str();
}

void SimpleMessage::setNames(unsigned int k, const char * names)
{
    if (k>=names_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", names_arraysize, k);
    this->names[k] = names;
}

class SimpleMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    SimpleMessageDescriptor();
    virtual ~SimpleMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(SimpleMessageDescriptor)

SimpleMessageDescriptor::SimpleMessageDescriptor() : omnetpp::cClassDescriptor("SimpleMessage", "omnetpp::cMessage")
{
    propertynames = nullptr;
}

SimpleMessageDescriptor::~SimpleMessageDescriptor()
{
    delete[] propertynames;
}

bool SimpleMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SimpleMessage *>(obj)!=nullptr;
}

const char **SimpleMessageDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *SimpleMessageDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int SimpleMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int SimpleMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISCOMPOUND,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *SimpleMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "pairs",
        "names",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int SimpleMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pairs")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "names")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *SimpleMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "MyPair",
        "string",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **SimpleMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *SimpleMessageDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int SimpleMessageDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    SimpleMessage *pp = (SimpleMessage *)object; (void)pp;
    switch (field) {
        case 0: return pp->getPairsArraySize();
        case 1: return pp->getNamesArraySize();
        default: return 0;
    }
}

const char *SimpleMessageDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SimpleMessage *pp = (SimpleMessage *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SimpleMessageDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SimpleMessage *pp = (SimpleMessage *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getPairs(i); return out.str();}
        case 1: return oppstring2string(pp->getNames(i));
        default: return "";
    }
}

bool SimpleMessageDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    SimpleMessage *pp = (SimpleMessage *)object; (void)pp;
    switch (field) {
        case 1: pp->setNames(i,(value)); return true;
        default: return false;
    }
}

const char *SimpleMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(MyPair));
        default: return nullptr;
    };
}

void *SimpleMessageDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    SimpleMessage *pp = (SimpleMessage *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getPairs(i)); break;
        default: return nullptr;
    }
}


