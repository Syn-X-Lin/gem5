from m5.SimObject import SimObject
from m5.params import *
from m5.proxy import *
from SimpleEnergySM import SimpleEnergySM

class EnergyMgmt(SimObject):
    type = 'EnergyMgmt'
    cxx_header = "engy/energy_mgmt.hh"
    path_energy_profile = Param.String(Parent.path_energy_profile, "path to energy profile")
    energy_time_unit = Param.Int(Parent.energy_time_unit, "time unit for energy profile")
    energy_modules = Param.String(Parent.energy_modules, "modules connected to energy port")
    state_machine = Param.BaseEnergySM(SimpleEnergySM(), "energy state machine")