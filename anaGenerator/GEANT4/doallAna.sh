export TESTBEAM=list/Filelist_GEANT4_Electron_Ar500MeV.txt
#list/Filelist_GEANT4_Pion_Ar_1GeV_1mstat.txt
#Filelist_GEANT4_Pion_Ar_1GeV_v010000xstat.txt
#list/Filelist_GEANT4_Electron_Ar1GeV_v010000xstat.txt

ntostop=
#11900000
#10000000

tag=TESTBEAM_Electron_500MeV-Ar_HASthreshold${ntostop}
#TESTBEAM_Piplus-1GeV-Ar_1mstat_HASthreshold${ntostop}
#TESTBEAM_Piplus-1GeV-Ar_10000xstat_HASthreshold_Allisgood_Allsave_testNucleiSetIDandSkip
#TESTBEAM_Electron-1GeV-Ar_10000xstat_HASthreshold_Allisgood_Allsave_testNucleiSetIDandSkip

#TESTBEAM_Ar_10000xstat_HASthreshold_Allisgood
#TESTBEAM_testElectron1GeVV0_10000xstat_0threshold

####################################

mkexe.sh anaGenerator -I${NUGENTKI}/include

#exit

# TESTBEAM = 11
opt=${tag}TESTBEAM;          nohup ./doAna.sh $TESTBEAM         ${opt} 11 ${ntostop} > see${opt}.log &

#exit
