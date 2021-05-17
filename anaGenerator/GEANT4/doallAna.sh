export GEANT4CHARGEDBEAM="list/Filelist_GEANT4_Ar_v010000xstat.txt"
#"list/Filelist_GEANT4_v0test_1000xstat.txt"

ntostop=
#10000000

tag=GEANT4CHARGEDBEAM_testArv1_10000xstat_0threshold

####################################

mkexe.sh anaGenerator -I${NUGENTKI}/include

#exit

# GEANT4CHARGEDBEAM = 11
opt=${tag}GEANT4CHARGEDBEAM;          nohup ./doAna.sh $GEANT4CHARGEDBEAM         ${opt} 11 ${ntostop} > see${opt}.log &

#exit
