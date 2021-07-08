export GEANT4CHARGEDBEAM="list/Filelist_FLUKA_Pion_v01GeVc.txt"
#"list/Filelist_GEANT4_Electron_1GeV_v010000xstat.txt"
#"list/Filelist_GEANT4_Electron_v010000xstat.txt"
#
#"list/Filelist_GEANT4_v0test_1000xstat.txt"

ntostop=
#10000000

tag=GEANT4CHARGEDBEAM_Piplus-1GeV-Ar_10000xstat_HASthreshold_Allisgood_Allsave
#GEANT4CHARGEDBEAM_Ar_10000xstat_HASthreshold_Allisgood
#GEANT4CHARGEDBEAM_testElectron1GeVV0_10000xstat_0threshold

####################################

mkexe.sh anaGenerator -I${NUGENTKI}/include

#exit

# GEANT4CHARGEDBEAM = 11
opt=${tag}GEANT4CHARGEDBEAM;          nohup ./doAna.sh $GEANT4CHARGEDBEAM         ${opt} 11 ${ntostop} > see${opt}.log &

#exit
