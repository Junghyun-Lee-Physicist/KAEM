#! /bin/bash

mkdir merge

#for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25

do

cd Compile${i}/
mv Mirror.root ../merge/${i}.root
#This code rood result name is "TargetOpti"!!!!
#CAUTION
cd ..

done

rm -rf Compile*
cd merge
hadd Mirror.root *.root
mv Mirror.root ../
cd ../
rm -rf merge
