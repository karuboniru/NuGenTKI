mkexe.sh draw -lstyle -I${NUGENTKI}/style -L${NUGENTKI}/style

task=_Wrest_xrest_Q2_All

outdir=outplot/outdraw${task}

mkdir -p ${outdir}

for infile in outplot/outAna11_TESTBEAM_HASthreshold_WrestXrestQ2All_Filelist_GEANT4_Electron_Ar1GeV_v010000xstatTESTBEAM_Filelist_GEANT4_Electron_Ar1GeV_v010000xstat.root outplot/outAna11_TESTBEAM_HASthreshold_WrestXrestQ2All_Filelist_GEANT4_Electron_Ar500MeVTESTBEAM_Filelist_GEANT4_Electron_Ar500MeV.root outplot/outAna11_TESTBEAM_HASthreshold_WrestXrestQ2All_Filelist_GEANT4_Piplus_Ar_1GeVc_1bTESTBEAM_Filelist_GEANT4_Piplus_Ar_1GeVc_1b.root
do

    tag=$(echo $infile | tr -s '/' '_' | tr -s '.' '_')$task
    echo $tag
    
    ./draw ${infile} ${outdir} > seedraw${tag}.log &
done


