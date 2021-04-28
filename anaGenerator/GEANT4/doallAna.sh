export GEANT4PIPLUSKE1GEV="list/Filelist_GEANT4_v0test_1000xstat.txt"

ntostop=
#10000000

tag=GEANT4PIPLUSKE1GEV_test0_1000xstat_0threshold

####################################

mkexe.sh anaGenerator -I${NUGENTKI}/include

#exit

# GEANT4PIPLUSKE1GEV = 11
opt=${tag}GEANT4PIPLUSKE1GEV;          nohup ./doAna.sh $GEANT4PIPLUSKE1GEV         ${opt} 11 ${ntostop} > see${opt}.log &

#exit
