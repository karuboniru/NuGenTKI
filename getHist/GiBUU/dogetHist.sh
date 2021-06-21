export GFSa9t1nuH=
export GFSa9t1nuC="outplot/outAna9_MINERvALE_GiBUU_newE_GFS0PIa9nuC_GiBUUMINERvA_LE_Neutrino_current_Carbon_250files.root"
#"outplot/outAna9_MINERvALE_GiBUU_repeatPRDoldE_GFS0PIa9nuC_GiBUUMINERvA_LE_Neutrino_current_Carbon_250files.root"
#"outplot/outAna9_MINERvALE_GiBUU_test_GFS0PIa9nuC_Filelist_GiBUUMINERvALE_nu_T0_Carbon.root"
#"outplot/outAna9_MINERvAGiBUU_LE_GFS0PIa9nuC_GiBUUMINERvA_LE_Neutrino_more_ensembles_Carbon_250files.root"
export GFSa9t1nuNOFSIC=

export GFSPIZEROa7t4nuH=
export GFSPIZEROa7t4nuC="outplot/outAna7_MINERvALE_GiBUU_newE_GFSPIZEROa7nuC_GiBUUMINERvA_LE_Neutrino_current_Carbon_250files.root"
#"outplot/outAna7_MINERvALE_GiBUU_repeatPRDoldE_GFSPIZEROa7nuC_GiBUUMINERvA_LE_Neutrino_current_Carbon_250files.root"
#"outplot/outAna7_MINERvALE_GiBUU_test_GFSPIZEROa7nuC_Filelist_GiBUUMINERvALE_nu_T0_Carbon.root"
#"outplot/outAna7_MINERvAGiBUU_LE_GFSPIZEROa7nuC_GiBUUMINERvA_LE_Neutrino_more_ensembles_Carbon_250files.root"
export GFSPIZEROa7t4nuNOFSIC=

export GFSt2nuC=
export LOWRECOILt8nubarC=""
export LOWRECOILt8nubarH=""
export LOWRECOILt8nuC=""
export LOWRECOILt8nuH=""
export NUBAR1PIt16nubarC=""
export NUBAR1PIt16nubarH=""

export MMECCQEt32nuC=

export GFSa3t2nuC=

export DUNEEXCL3a10t5nuH1=
export DUNEEXCL3a10t6nuH4=${DUNEEXCL3a10t5nuH1}

export DUNEEXCL3a10t5nubarH1=
export DUNEEXCL3a10t6nubarH4=${DUNEEXCL3a10t5nubarH1}

export DUNEEXCL3a10t5nuC=

export DUNEEXCL3a10t5nubarC=

export tag=MINERvALE_GiBUU_newE__
#MINERvALE_GiBUU_repeatPRDoldE__
#MINERvAGiBUU_LE_

#######################################################################################
mkexe.sh getHist -I${NUGENTKI}/include

#exit

#GFS 1

#opt=${tag}GFSa9t1nuH;               nohup ./getHist $GFSa9t1nuH ${opt} 1  > see${opt}.log &
 opt=${tag}GFSa9t1nuC;               nohup ./getHist $GFSa9t1nuC ${opt} 1  > see${opt}.log &
#opt=${tag}GFSa9t1nuNOFSIC;          nohup ./getHist $GFSa9t1nuNOFSIC ${opt} 1  > see${opt}.log &

#exit

#GFS 4 for GFSPIZEROa7 inclusive pi0

#opt=${tag}GFSPIZEROa7t4nuH;            nohup ./getHist $GFSPIZEROa7t4nuH            ${opt} 4   > see${opt}.log &
 opt=${tag}GFSPIZEROa7t4nuC;            nohup ./getHist $GFSPIZEROa7t4nuC            ${opt} 4   > see${opt}.log &
#opt=${tag}GFSPIZEROa7t4nuNOFSIC;       nohup ./getHist $GFSPIZEROa7t4nuNOFSIC       ${opt} 4   > see${opt}.log &

exit

#EXCL 5 

opt=${tag}DUNEEXCL3a10t5nuH1;            nohup ./getHist $DUNEEXCL3a10t5nuH1            ${opt} 5   > see${opt}.log &
opt=${tag}DUNEEXCL3a10t6nuH4;            nohup ./getHist $DUNEEXCL3a10t6nuH4            ${opt} 6   > see${opt}.log &

opt=${tag}DUNEEXCL3a10t5nubarH1;            nohup ./getHist $DUNEEXCL3a10t5nubarH1            ${opt} 5   > see${opt}.log &
opt=${tag}DUNEEXCL3a10t6nubarH4;            nohup ./getHist $DUNEEXCL3a10t6nubarH4            ${opt} 6   > see${opt}.log &

opt=${tag}DUNEEXCL3a10t5nuC;               nohup ./getHist $DUNEEXCL3a10t5nuC               ${opt} 5   > see${opt}.log &
opt=${tag}DUNEEXCL3a10t5nubarC;            nohup ./getHist $DUNEEXCL3a10t5nubarC            ${opt} 5   > see${opt}.log &

exit

#MMECCQE 32
opt=${tag}MMECCQEt32nuC; nohup ./getHist $MMECCQEt32nuC ${opt} 32  > see${opt}.log &

exit


#GFS 2 for GFSEXP(3)
opt=${tag}GFSa3t2nuC;          nohup ./getHist $GFSa3t2nuC          ${opt} 2   > see${opt}.log &

exit


#GFS 1 2 both carbon only
opt=${tag}GFSt1nuC; nohup ./getHist $GFSt1nuC ${opt} 1  > see${opt}.log &
opt=${tag}GFSt2nuC; nohup ./getHist $GFSt2nuC ${opt} 2  > see${opt}.log &

#LOWRECOIL 8
opt=${tag}LOWRECOILt8nubarC; nohup ./getHist $LOWRECOILt8nubarC ${opt} 8  > see${opt}.log &
opt=${tag}LOWRECOILt8nubarH; nohup ./getHist $LOWRECOILt8nubarH ${opt} 8  > see${opt}.log &
opt=${tag}LOWRECOILt8nuC;    nohup ./getHist $LOWRECOILt8nuC    ${opt} 8  > see${opt}.log &
opt=${tag}LOWRECOILt8nuH;    nohup ./getHist $LOWRECOILt8nuH    ${opt} 8  > see${opt}.log &

#NUBAR1PI 16
opt=${tag}NUBAR1PIt16nubarC; nohup ./getHist $NUBAR1PIt16nubarC ${opt} 16  > see${opt}.log &
opt=${tag}NUBAR1PIt16nubarH; nohup ./getHist $NUBAR1PIt16nubarH ${opt} 16  > see${opt}.log &

exit
