mkexe.sh draw -lstyle -I${NUGENTKI}/style -L${NUGENTKI}/style

infile=outplot/outAna11_TESTBEAM_Piplus-1GeV-Ar_10000xstat_HASthreshold_Allisgood_Allsave_testNucleiSetIDandSkipTESTBEAM_Filelist_GEANT4_Pion_Ar_1GeV_v010000xstat.root

#outAna11_TESTBEAM_Piplus-1GeV-Ar_10000xstat_HASthreshold_Allisgood_AllsaveTESTBEAM_Filelist_GEANT4_Pion_v010000xstat.root

outdir=outplot/outdraw

mkdir -p ${outdir}

./draw ${infile} ${outdir} > seedraw.log &


