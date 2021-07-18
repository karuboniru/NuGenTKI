mkexe.sh draw -lstyle -I${NUGENTKI}/style -L${NUGENTKI}/style

task=_Wrest_xrest_1b

outdir=outplot/outdraw${task}

mkdir -p ${outdir}

#    outplot/outAna11_TESTBEAM_Electron-1GeV-Ar_10000xstat_HASthreshold_Allisgood_Allsave_testNucleiSetIDandSkipTESTBEAM_Filelist_GEANT4_Electron_Ar1GeV_v010000xstat.root 
for infile in outplot/outAna11_TESTBEAM_Piplus-1GeVc-Ar_1bstat_HASthreshold_WrestXrestTESTBEAM_Filelist_GEANT4_Piplus_Ar_1GeVc_1b.root
    #outplot/outAna11_TESTBEAM_Piplus-1GeV-Ar_10000xstat_HASthreshold_Wrest_xrestTESTBEAM_Filelist_GEANT4_Pion_Ar_1GeV_v010000xstat.root outplot/outAna11_TESTBEAM_Electron-1GeV-Ar_10000xstat_HASthreshold_Wrest_xrestTESTBEAM_Filelist_GEANT4_Electron_Ar1GeV_v010000xstat.root
    #outplot/outAna11_TESTBEAM_Electron_500MeV-Ar_HASthreshold_checkTESTBEAM_Filelist_GEANT4_Electron_Ar500MeV.root outplot/outAna11_TESTBEAM_Piplus-1GeVc-Ar_1bstat_HASthresholdTESTBEAM_Filelist_GEANT4_Piplus_Ar_1GeVc_1b.root
    #outplot/outAna11_TESTBEAM_Piplus-1GeV-Ar_10000xstat_HASthreshold_checkTESTBEAM_Filelist_GEANT4_Pion_Ar_1GeV_v010000xstat.root
    #outplot/outAna11_TESTBEAM_Electron_500MeV-Ar_HASthresholdTESTBEAM_Filelist_GEANT4_Electron_Ar500MeV.root
do

    tag=$(echo $infile | tr -s '/' '_' | tr -s '.' '_')
    echo $tag
    
    ./draw ${infile} ${outdir} > seedraw${tag}.log &
done


