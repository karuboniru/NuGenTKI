mkexe.sh draw -lstyle -I${NUGENTKI}/style -L${NUGENTKI}/style || exit

task=_IsGoodFilter_Wrest_xrest_Q2_SPK_All_10files_10b

outdir=outplot/outdraw${task}

mkdir -p ${outdir}

for infile in outplot/OutplotList_outAna11_TESTBEAM_IsGoodFilter_HASthreshold__WrestXrestQ2SPKAll_Filelist_GEANT4_Piplus_Ar_1GeVc_2TESTBEAM.log
#outplot/OutplotList_outAna11_TESTBEAM_HASthreshold_WrestXrestQ2SPKAll_Filelist_GEANT4_Piplus_Ar_1GeVc_2TESTBEAM.log
#outplot/outAna11_TESTBEAM_HASthreshold_WrestXrestQ2SPKAll_Filelist_GEANT4_Piplus_Ar_1GeVc_0TESTBEAM_Filelist_GEANT4_Piplus_Ar_1GeVc_0.root
#outplot/outAna11_TESTBEAM_HASthreshold_WrestXrestQ2SPKAll_Filelist_GEANT4_Electron_Ar1GeV_v010000xstatTESTBEAM_Filelist_GEANT4_Electron_Ar1GeV_v010000xstat.root outplot/outAna11_TESTBEAM_HASthreshold_WrestXrestQ2SPKAll_Filelist_GEANT4_Electron_Ar500MeVTESTBEAM_Filelist_GEANT4_Electron_Ar500MeV.root outplot/outAna11_TESTBEAM_HASthreshold_WrestXrestQ2SPKAll_Filelist_GEANT4_Piplus_Ar_1GeVc_1bTESTBEAM_Filelist_GEANT4_Piplus_Ar_1GeVc_1b.root
do

    tag=$(echo $infile | tr -s '/' '_' | tr -s '.' '_')$task
    echo $tag
    
    ./draw ${infile} ${outdir} > seedraw${tag}.log &
done


