//
// Created by lf-z on 12/28/16.
//
#include <fstream>
#include "engy/energy_mgmt.hh"
#include "debug/EnergyMgmt.hh"

EnergyMgmt::EnergyMgmt(const Params *p)
        : SimObject(p),
          time_unit(p->energy_time_unit),
          energy_remained(0),
          event_poweroff(this),
          event_poweron(this),
          event_energy_harvest(this),
          _path_energy_profile(p->path_energy_profile)
{

}

EnergyMgmt::~EnergyMgmt()
{

}

void EnergyMgmt::init()
{
    /* Read energy profile */
    energy_harvest_data = readEnergyProfile();
    /* Reset energy remained to 0. */
    energy_remained = 0;


    DPRINTF(EnergyMgmt, "Energy Management module initialized!\n");
    DPRINTF(EnergyMgmt, "Energy profile: %s (Time unit: %d ticks)\n",
            _path_energy_profile.c_str(), time_unit);

    /* Trigger first energy harvest event here */
    energyHarvest();

}

int EnergyMgmt::consumeEnergy(double val)
{
    /* Todo: Pass the module which consumed the energy to this function. (Or DPRINTF in the module which consumes energy) */
    /* Consume energy if val > 0, and harvest energy if val < 0 */
    energy_remained -= val;
    if (val > 0)
        DPRINTF(EnergyMgmt, "Energy %lf is consumed by xxx. Energy remained: %lf\n", val, energy_remained);
    else
        DPRINTF(EnergyMgmt, "Energy %lf is harvested. Energy remained: %lf\n", -val, energy_remained);
    return 1;
}

void EnergyMgmt::broadcastPowerOff()
{
    EnergyMsg msg;
    msg.type = POWEROFF;
    _meport.broadcastMsg(msg);
    DPRINTF(EnergyMgmt, "Insufficient energy, system power off.\n");
}

void EnergyMgmt::broadcastPowerOn()
{
    EnergyMsg msg;
    msg.type = POWERON;
    _meport.broadcastMsg(msg);
    DPRINTF(EnergyMgmt, "Sufficient energy, system power on.\n");
}

std::vector<double> EnergyMgmt::readEnergyProfile()
{
    std::vector<double> data;
    double temp;
    std::ifstream fin;
    fin.open(_path_energy_profile.c_str());
    /* Read energy profile and store the data into vector. */
    data.resize(0);
    while (fin>>temp) {
        data.push_back(temp);
    }
    /* Reverse the energy harvest queue so that the first energy unit pops first */
    reverse(data.begin(), data.end());
    fin.close();
    return data;
}

void EnergyMgmt::energyHarvest()
{
    /* Add harvested energy into capacity. */
    double energy_val = energy_harvest_data.back();
    consumeEnergy(-energy_val);
    energy_harvest_data.pop_back();

    /* Trigger the next harvest function. */
    schedule(event_energy_harvest, curTick() + time_unit);

    // DPRINTF(EnergyMgmt, "Energy %lf harvested.\n", energy_val);
}

EnergyMgmt *
EnergyMgmtParams::create()
{
    return new EnergyMgmt(this);
}
