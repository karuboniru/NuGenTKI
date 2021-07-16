mkexe.sh draw -lstyle -I${NUGENTKI}/style -L${NUGENTKI}/style

outdir=outplot/outdraw

mkdir -p ${outdir}

for infile in outplot/outAna11_TESTBEAM_Piplus-1GeVc-Ar_HASthresholdTESTBEAM_Filelist_FLUKA_Pion_v01GeVc.root
    #outplot/outAna11_TESTBEAM_Piplus-1GeV-Ar_test_HASthreshold_Allisgood_Allsave_repTESTBEAM_Filelist_FLUKA_Pion_v01GeVc.root
do

    tag=$(echo $infile | tr -s '/' '_' | tr -s '.' '_')
    echo $tag
    
    ./draw ${infile} ${outdir} > seedraw${tag}.log &
done

