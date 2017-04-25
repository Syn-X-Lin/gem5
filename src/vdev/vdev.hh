//
// Created by lf-z on 4/24/17.
//

#ifndef GEM5_VDEV_HH
#define GEM5_VDEV_HH

#include <string>
#include <vector>
#include "mem/mem_object.hh"
#include "cpu/base.hh"
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
    BaseCPU *cpu;
    /** Address range of the virtual device*/
    AddrRange range;
    /** Trace of the vdev, used to determine whether a task has succeeded */
    std::vector<std::string> trace;
    /** The delay for the CPU to set the vdev **/
    Tick delay_set;
    /** The delay for the vdev to finish its task **/
    Tick delay_self;
    /** The delay for the CPU to handle the interrupt caused by the vdev */
    Tick delay_cpu_interrupt;
    /** Whether the tasks of the vdev is interruptable */
    bool is_interruptable;
    /** When power off, time is remained for the task */
    Tick delay_remained;
    /** Whether the device is in a task */
    bool is_intask;
    EventWrapper<VirtualDevice, &VirtualDevice::triggerInterrupt> event_interrupt;
    /**Tell whether the task is successful */
    virtual bool finishSuccess();
};

#endif //GEM5_VDEV_HH
