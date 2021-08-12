mkexe.sh anaGenerator -I${NUGENTKI}/include -lstyle -I${NUGENTKI}/style  -L${NUGENTKI}/style || exit

for TESTBEAM in list/Filelist*

#list/Filelist_GEANT4_Piplus_Ar_1GeVc_1b.txt list/Filelist_GEANT4_Electron_Ar1GeV_v010000xstat.txt list/Filelist_GEANT4_Electron_Ar500MeV.txt
do

ntostop=
#11900000
#10000000

tag=TESTBEAM_IsGoodFilter_HASthreshold_${ntostop}_WrestXrestQ2SPKAll_$(echo $TESTBEAM | awk -F\/ '{print $2}'| awk -F\. '{print $1}')

####################################

echo $tag

# TESTBEAM = 11
opt=${tag}TESTBEAM;          nohup ./doAna.sh $TESTBEAM         ${opt} 11 ${ntostop} > see${opt}.log &

done

