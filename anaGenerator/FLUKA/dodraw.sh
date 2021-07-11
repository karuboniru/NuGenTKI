mkexe.sh draw -lstyle -I${NUGENTKI}/style -L${NUGENTKI}/style

infile=outplot/outAna11_TESTBEAM_Piplus-1GeV-Ar_test_HASthreshold_Allisgood_AllsaveTESTBEAM_Filelist_FLUKA_Pion_v01GeVc.root

outdir=outplot/outdraw

mkdir -p ${outdir}

./draw ${infile} ${outdir} > seedraw.log &


