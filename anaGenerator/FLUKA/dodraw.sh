mkexe.sh draw -I${NUGENTKI}/include -lstyle -I${NUGENTKI}/style -L${NUGENTKI}/style || exit

task=smear_IsGoodFilter_WrestXrestQ2SPK
#_NOFSI10Files_IsGoodFilter_WrestXrestQ2SPK
outdir=outplot/outdraw${task}

mkdir -p ${outdir}

for infile in outplot/outAna11_TESTBEAM_smeared_Piplus-1GeVc-Ar_IsGoodFilter_HASthreshold_WrestXrestQ2SPKTESTBEAM_Filelist_FLUKA_Pion_v01GeVc_18files.root
    #outplot/outAna11_TESTBEAM_Piplus-1GeVc-Ar_NOFSI10Files_IsGoodFilter_HASthreshold_WrestXrestQ2SPKTESTBEAM_Filelist_FLUKA_Pion_v01GeVc_NOFSI_10file.root
    #outplot/outAna11_TESTBEAM_Piplus-1GeVc-Ar_NOFSI_IsGoodFilter_HASthreshold_WrestXrestQ2SPKTESTBEAM_Filelist_FLUKA_Pion_v01GeVc_NOFSI_10files.root
    #outplot/outAna11_TESTBEAM_Piplus-1GeVc-Ar_IsGoodFilter_HASthreshold_WrestXrestQ2SPKTESTBEAM_Filelist_FLUKA_Pion_v01GeVc_18files.root
    
    #outplot/outAna11_TESTBEAM_Piplus-1GeVc-Ar_HASthreshold_WrestXrestQ2SPKTESTBEAM_Filelist_FLUKA_Pion_v01GeVc.root
    #outplot/outAna11_TESTBEAM_Piplus-1GeVc-Ar_HASthreshold_WrestXrestTESTBEAM_Filelist_FLUKA_Pion_v01GeVc.root
    #outplot/outAna11_TESTBEAM_Piplus-1GeVc-Ar_HASthresholdTESTBEAM_Filelist_FLUKA_Pion_v01GeVc.root
    #outplot/outAna11_TESTBEAM_Piplus-1GeV-Ar_test_HASthreshold_Allisgood_Allsave_repTESTBEAM_Filelist_FLUKA_Pion_v01GeVc.root
do

    tag=$(echo $infile | tr -s '/' '_' | tr -s '.' '_')$task
    echo $tag
    
    ./draw ${infile} ${outdir} > seedraw${tag}.log &
done

