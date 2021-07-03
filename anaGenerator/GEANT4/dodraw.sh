mkexe.sh draw -lstyle -I${NUGENTKI}/style -L${NUGENTKI}/style

infile=outplot/outAna11_GEANT4CHARGEDBEAM_Piplus-1GeV-Ar_10000xstat_HASthreshold_Allisgood_AllsaveGEANT4CHARGEDBEAM_Filelist_GEANT4_Pion_v010000xstat.root

outdir=outplot/outdraw

mkdir -p ${outdir}

./draw ${infile} ${outdir}

