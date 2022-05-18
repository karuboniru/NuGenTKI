export GFS0PIa9t1nuCarbon="outplot/outAna9_MINERvANuWro_test_GFS0PIa9nuCH_Filelist_NuWro_test_BeamNu_EnuMINERvA_TargetCarbon.root"
#"outplot/outAna9_MINERvAGENIE_v2TJ_LE_GFS0PIa9nuCH_Filelist_GENIE_v2TJ_BeamNu_EnuMINERvA_TargetCarbon.root"
#"outplot/outAna9_MINERvAGENIE_v2DC_LE_GFSa9nuCH_Filelist_GENIE_v2DC_BeamNu_EnuMINERvA_TargetCarbon.root"
#"outplot/outAna9_MINERvANuWro_OOB_LE_GFSa9nuCH_Filelist_NuWro_OOB_BeamNu_EnuMINERvA_TargetCarbon.root"
#"outplot/outAna9_MINERvAGENIE_V2RG_LE_GFSa9nuCH_Filelist_GENIE_V2RG_BeamNu_EnuMINERvA_TargetCarbon.root"
export GFS0PIa9t1nuCH=

export GFSPIZEROa7t4nuCarbon="outplot/outAna7_MINERvANuWro_test_GFSPIZEROa7nuCH_Filelist_NuWro_test_BeamNu_EnuMINERvA_TargetCarbon.root"
#"outplot/outAna7_MINERvAGENIE_v2TJ_LE_GFSPIZEROa7nuCH_Filelist_GENIE_v2TJ_BeamNu_EnuMINERvA_TargetCarbon.root"
#"outplot/outAna7_MINERvAGENIE_v2DC_LE_GFSPIZEROa7nuCH_Filelist_GENIE_v2DC_BeamNu_EnuMINERvA_TargetCarbon.root"
#"outplot/outAna7_MINERvANuWro_OOB_LE_GFSPIZEROa7nuCH_Filelist_NuWro_OOB_BeamNu_EnuMINERvA_TargetCarbon.root"
#"outplot/outAna7_MINERvAGENIE_V2RG_LE_GFSPIZEROa7nuCH_Filelist_GENIE_V2RG_BeamNu_EnuMINERvA_TargetCarbon.root"
export GFSPIZEROa7t4nuCH=

export GFSt2nuCH=${GFSt1nuCH}

export LOWRECOILt8nuCH=

export LOWRECOILt8nubarCH=

export NUBAR1PIt16nubarCH=

export CLRt1nuCH=

export CLRt2nuCH=${CLRt1nuCH}

export MMECCQEt32nuCH=

export GFSEXPa3t2nuCH=

export GFSPIZEROa7t2nuCH=

export tag=MINERvANuWro_test_
#MINERvAGENIE_v2TJ_LE_
#MINERvAGENIE_v2DC_LE_
#MINERvANuWro_OOB_LE_
#MINERvAGENIE_V2RG_LE_

#######################################################################################

mkexe.sh getHist -I${NUGENTKI}/include

#GFS 4 for GFSPIZEROa7

opt=${tag}GFSPIZEROa7t4nuCarbon;    nohup ./getHist $GFSPIZEROa7t4nuCarbon    ${opt} 4   > see${opt}.log &

#exit

#GFS 1 for GFS0PIa9t1

opt=${tag}GFS0PIa9t1nuCarbon;       nohup ./getHist $GFS0PIa9t1nuCarbon       ${opt} 1   > see${opt}.log &

exit

#GFS 2 for GFSPIZEROa7 only testing!

opt=${tag}GFSPIZEROa7t2nuCH;       nohup ./getHist $GFSPIZEROa7t2nuCH       ${opt} 2   > see${opt}.log &

exit

#MMECCQE 32
opt=${tag}MMECCQEt32nuCH; nohup ./getHist $MMECCQEt32nuCH ${opt} 32  > see${opt}.log &

exit


#GFS 2 for GFSEXP(3)
opt=${tag}GFSEXPa3t2nuCH;          nohup ./getHist $GFSEXPa3t2nuCH          ${opt} 2   > see${opt}.log &

exit

#CLR 1 2, use the same getHist as GFS1
opt=${tag}CLRt1nuCH;          nohup ./getHist $CLRt1nuCH          ${opt} 1   > see${opt}.log &

opt=${tag}CLRt2nuCH;          nohup ./getHist $CLRt2nuCH          ${opt} 2   > see${opt}.log &

exit

#GFS 1 2 
opt=${tag}GFSt1nuCH;          nohup ./getHist $GFSt1nuCH          ${opt} 1   > see${opt}.log &
opt=${tag}GFSt2nuCH;          nohup ./getHist $GFSt2nuCH          ${opt} 2   > see${opt}.log &

#LOWRECOIL 8
opt=${tag}LOWRECOILt8nuCH;    nohup ./getHist $LOWRECOILt8nuCH    ${opt} 8   > see${opt}.log &
opt=${tag}LOWRECOILt8nubarCH; nohup ./getHist $LOWRECOILt8nubarCH ${opt} 8   > see${opt}.log &

#NUBAR1PI 16
opt=${tag}NUBAR1PIt16nubarCH; nohup ./getHist $NUBAR1PIt16nubarCH ${opt} 16  > see${opt}.log &

exit

