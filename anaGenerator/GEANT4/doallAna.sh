export TESTBEAM=list/Filelist_GEANT4_Electron_Ar1GeV_v010000xstat.txt
#list/Filelist_GEANT4_Pion_Ar_1GeV_v010000xstat.txt
#list/Filelist_GEANT4_Electron_Ar500MeV.txt
#
#list/Filelist_GEANT4_Piplus_Ar_1GeVc_1b.txt
#

ntostop=
#11900000
#10000000

tag=TESTBEAM_Electron-1GeV-Ar_10000xstat_HASthreshold_newname
#TESTBEAM_Piplus-1GeV-Ar_10000xstat_HASthreshold_newname
#TESTBEAM_Electron_500MeV-Ar_HASthreshold${ntostop}_newname
#
#TESTBEAM_Piplus-1GeVc-Ar_1bstat_HASthreshold${ntostop}

#TESTBEAM_Piplus-1GeV-Ar_10000xstat_HASthreshold_Allisgood_Allsave_testNucleiSetIDandSkip
#

#TESTBEAM_Ar_10000xstat_HASthreshold_Allisgood
#TESTBEAM_testElectron1GeVV0_10000xstat_0threshold

####################################

mkexe.sh anaGenerator -I${NUGENTKI}/include -lstyle -I${NUGENTKI}/style  -L${NUGENTKI}/style || return 1

#exit

# TESTBEAM = 11
opt=${tag}TESTBEAM;          nohup ./doAna.sh $TESTBEAM         ${opt} 11 ${ntostop} > see${opt}.log &

#exit
