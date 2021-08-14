export TESTBEAM=list/Filelist_FLUKA_Pion_v01GeVc_NOFSI_10files.txt
#"list/Filelist_FLUKA_Pion_v01GeVc_18files.txt"

#"list/Filelist_GEANT4_Electron_1GeV_v010000xstat.txt"
#"list/Filelist_GEANT4_Electron_v010000xstat.txt"
#
#"list/Filelist_GEANT4_v0test_1000xstat.txt"

ntostop=
#10000000

tag=TESTBEAM_Piplus-1GeVc-Ar_NOFSI_IsGoodFilter_HASthreshold_WrestXrestQ2SPK
#TESTBEAM_Ar_10000xstat_HASthreshold_Allisgood
#TESTBEAM_testElectron1GeVV0_10000xstat_0threshold

####################################

mkexe.sh anaGenerator -I${NUGENTKI}/include -lstyle -I${NUGENTKI}/style  -L${NUGENTKI}/style || exit

#exit

# TESTBEAM = 11
opt=${tag}TESTBEAM;          nohup ./doAna.sh $TESTBEAM         ${opt} 11 ${ntostop} > see${opt}.log &

#exit
