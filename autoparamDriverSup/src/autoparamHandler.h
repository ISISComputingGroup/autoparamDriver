#pragma once

// Standard includes
#include <string>
#include <vector>

// EPICS includes
#include <asynPortDriver.h>
#include <alarm.h>
#include <epicsTypes.h>

namespace Autoparam {

class Reason {
  public:
    explicit Reason(char const *asynReason);

    Reason(Reason const &other);

    Reason &operator=(Reason const &other);

    // Najbrž bo derived driver rabil kaj več kot to, kar nudi Reason, tako da
    // pričakujemo dedovanje
    virtual ~Reason();

    std::string const &function() const { return m_function; }

    std::vector<std::string> const &arguments() const { return m_arguments; }

    std::string normalized() const;

  private:
    friend class Driver;

    void setIndex(int index) { m_asynParamIndex = index; }

    int index() const { return m_asynParamIndex; }

    int m_asynParamIndex;
    std::string m_function;
    std::vector<std::string> m_arguments;
};

template <typename T> class Array {
  public:
    Array() : m_data(NULL), m_size(0) {}
    Array(T *value, size_t size) : m_data(value), m_size(size) {}
    Array(std::vector<T> &vector)
        : m_data(vector.data()), m_size(vector.size) {}

    T *data() const { return m_data; }

    size_t size() const { return m_size; }

  private:
    T *m_data;
    size_t m_size;
};

struct ResultBase {
    asynStatus status;
    epicsAlarmCondition alarmStatus;
    epicsAlarmSeverity alarmSeverity;

    ResultBase()
        : status(asynStatus((-1))), alarmStatus(epicsAlarmCondition(-1)),
          alarmSeverity(epicsAlarmSeverity(-1)) {}
};

template <typename T = void> struct Result : ResultBase {
    T value;

    Result() : ResultBase(), value() {}
};

template <> struct Result<void> : ResultBase {};

template <> struct Result<epicsUInt32> : ResultBase {
    epicsUInt32 value;
    epicsUInt32 mask;

    Result() : value(), mask() {}
};

template <typename T> struct AsynType;

template <typename T> struct Handlers {
    typedef Result<void> WriteResult;
    typedef Result<T> ReadResult;
    typedef WriteResult (*WriteHandler)(Reason &, T);
    typedef ReadResult (*ReadHandler)(Reason &);

    static const asynParamType type = AsynType<T>::value;
    WriteHandler writeHandler;
    ReadHandler readHandler;

    Handlers() : writeHandler(NULL), readHandler(NULL) {}
};

typedef std::string const &Octet;

typedef Handlers<epicsInt32> Int32Handlers;
template <> struct AsynType<epicsInt32> {
    static const asynParamType value = asynParamInt32;
};

typedef Handlers<epicsInt64> Int64Handlers;
template <> struct AsynType<epicsInt64> {
    static const asynParamType value = asynParamInt64;
};

typedef Handlers<epicsUInt32> UInt32DigitalHandlers;
template <> struct AsynType<epicsUInt32> {
    static const asynParamType value = asynParamUInt32Digital;
};

typedef Handlers<epicsFloat64> Float64Handlers;
template <> struct AsynType<epicsFloat64> {
    static const asynParamType value = asynParamFloat64;
};

typedef Handlers<Octet> OctetHandlers;
template <> struct AsynType<Octet> {
    static const asynParamType value = asynParamOctet;
};

typedef Handlers<Array<epicsInt8> > Int8ArrayHandlers;
template <> struct AsynType<Array<epicsInt8> > {
    static const asynParamType value = asynParamInt8Array;
};

typedef Handlers<Array<epicsInt16> > Int16ArrayHandlers;
template <> struct AsynType<Array<epicsInt16> > {
    static const asynParamType value = asynParamInt16Array;
};

typedef Handlers<Array<epicsInt32> > Int32ArrayHandlers;
template <> struct AsynType<Array<epicsInt32> > {
    static const asynParamType value = asynParamInt32Array;
};

typedef Handlers<Array<epicsInt64> > Int64ArrayHandlers;
template <> struct AsynType<Array<epicsInt64> > {
    static const asynParamType value = asynParamInt64Array;
};

typedef Handlers<Array<epicsFloat32> > Float32ArrayHandlers;
template <> struct AsynType<Array<epicsFloat32> > {
    static const asynParamType value = asynParamFloat32Array;
};

typedef Handlers<Array<epicsFloat64> > Float64ArrayHandlers;
template <> struct AsynType<Array<epicsFloat64> > {
    static const asynParamType value = asynParamFloat64Array;
};

} // namespace Autoparam