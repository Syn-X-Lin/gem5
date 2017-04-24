//
// Created by lf-z on 4/24/17.
//

#ifndef GEM5_VDEV_HH
#define GEM5_VDEV_HH

#include <string>
#include <vector>
#include "mem/mem_object.hh"
#include "params/VirtualDevice.hh"

class VirtualDevice : public MemObject
{
public:
    typedef VirtualDeviceParams Params;
    const Params *params() const
    {
        return reinterpret_cast<const Params *>(_params);
    }
    VirtualDevice(const Params *p);
    virtual ~VirtualDevice() {}
    virtual void init();

    void triggerInterrupt();
    Tick access(PacketPtr pkt);
    virtual int handleMsg(const EnergyMsg &msg);
protected:
    SimObject *cpu;
    std::vector<std::string> trace;
    Tick delay_set;
    Tick delay_self;
    Tick delay_cpu_interrupt;
    EventWrapper<VirtualDevice, &VirtualDevice::triggerInterrupt> event_interrupt;
    virtual bool finish_success();
};

#endif //GEM5_VDEV_HH
