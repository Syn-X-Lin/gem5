#!/bin/sh

build/ARM/gem5.debug --debug-flags=EnergyMgmt --debug-file=a.out configs/example/se_engy.py -c tests/test-progs/hello/bin/arm/linux/hello --energy-prof=./profile/energy_prof --energy-modules='cpu'
