#! /bin/bash

#for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30;

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
do

mkdir Compile${i}/
cd Compile${i}/
cmake -DGeant4_DIR=/cvmfs/geant4.cern.ch/geant4/${version}/x86_64-centos7-gcc8-optdeb-MT/lib64/Geant4-${version2} ..
make -j16

macro="run.mac"
echo "/run/initialize" >> ${macro}
echo "/tracking/verbose 0" >> ${macro}
echo "/run/beamOn 10" >> ${macro}

#txtname="cout${i}.txt"

#nohup ./KAEM -m ${macro} -s ${i} >> ${txtname} &
nohup ./KAEM -m ${macro} -s ${i} &
echo -ne '\n'

cd ..

done
