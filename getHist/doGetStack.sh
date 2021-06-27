dir0=$(pwd)

for anaid in GFSPIZEROMINERvAGiBUULE_newEtestnewvar GFS0PIMINERvAGiBUULE_newEtestnewvar
#GFSPIZEROMINERvAGiBUULE_newE GFS0PIMINERvAGiBUULE_newE
#MINERvALEGiBUUGFS0PIa9t1nuCarbon MINERvALEGiBUUGFSPIZEROa7t4nuCarbon
#MINERvAGENIEGFSPIZEROa7t4nuCarbon MINERvAGENIEGFS0PIa9t1nuCarbon
#GFS0PIMINERvAGiBUULE_CarbonOnly GFSPIZEROMINERvAGiBUULE_CarbonOnly GFSPIZEROMINERvAGENIEv2TJ_LE_CarbonOnly GFS0PIMINERvAGENIEv2TJ_LE_CarbonOnly
#GFSPIZEROMINERvAGENIE_v2DC_LE_CarbonOnly GFS0PIMINERvAGENIE_v2DC_LE_CarbonOnly
#GFSPIZEROMINERvAGENIE_V2RG_LE_CarbonOnly GFS0PIMINERvAGENIE_V2RG_LE_CarbonOnly 
#GFSPIZEROMINERvAGENIE_v3OOB_LE_CarbonOnly GFS0PIMINERvAGENIE_v3OOB_LE_CarbonOnly


do

echo
echo "===============================================================" $anaid
cd $dir0

#directory will be made in getStack.C

nfile=0
for locdir in ${anaid}{,nu{,bar}}
do
    echo locdir $locdir
    echo
    fin=outplot/${locdir}.root

    if [ ! -e $fin ]
    then
        continue
    fi

    nfile=$( expr $nfile + 1 )
        
    ls $fin
    echo
    tag=$(basename $fin | awk -F\. '{print $1}')
    echo tag $tag
    echo

    commonVar="beamE muonmomentum muontheta Q2 xBj xrest Wtrue Wrest event_recoilP event_recoilM_full  event_recoilM_zoom  recoilP recoilM_full  recoilM_zoom  "

    if [  $anaid == GFS0PIMINERvAGENIE_v3OOB_LE_CarbonOnly -o $anaid == GFS0PIMINERvAGENIE_v2DC_LE_CarbonOnly -o $anaid == GFS0PIMINERvAGENIE_V2RG_LE_CarbonOnly -o $anaid == GFS0PIMINERvAGiBUULE_CarbonOnly -o $anaid == GFS0PIMINERvAGENIEv2TJ_LE_CarbonOnly -o $anaid == MINERvAGENIEGFS0PIa9t1nuCarbon  -o $anaid == MINERvALEGiBUUGFS0PIa9t1nuCarbon -o $anaid == GFS0PIMINERvAGiBUULE_newE -o $anaid == GFS0PIMINERvAGiBUULE_newEtestnewvar ]
    then
        varray=${commonVar}" protonmomentum protontheta dpt dphit dalphat IApN"
    elif [ $anaid == GFSPIZEROMINERvAGENIE_v3OOB_LE_CarbonOnly -o $anaid == GFSPIZEROMINERvAGENIE_v2DC_LE_CarbonOnly -o $anaid == GFSPIZEROMINERvAGENIE_V2RG_LE_CarbonOnly -o $anaid == GFSPIZEROMINERvAGiBUULE_CarbonOnly -o $anaid == GFSPIZEROMINERvAGENIEv2TJ_LE_CarbonOnly -o $anaid == MINERvAGENIEGFSPIZEROa7t4nuCarbon -o $anaid == MINERvALEGiBUUGFSPIZEROa7t4nuCarbon  -o $anaid == GFSPIZEROMINERvAGiBUULE_newE -o $anaid == GFSPIZEROMINERvAGiBUULE_newEtestnewvar ]
    then
        varray=${commonVar}" protonmomentum protontheta pionmomentum piontheta pionEk baryonmomentum baryontheta baryonmass dpt dphit dalphat IApN dpTT "

    elif [  $anaid == DUNEEXCL3_CH ]                                                                                           
    then                                                                                                                       
        varray=${commonVar}"protonmomentum protontheta pionEk pionmomentum piontheta baryonmomentum baryontheta baryonmass dpT\
T dpTT20s dpTT10s dpTT5s dpTT2s "
    elif [ $anaid == LOWRECOILMINERvAGENIEOOB -o $anaid == LOWRECOILMINERvAGENIEISO  -o $anaid == LOWRECOILMINERvAGENIEISOw2 ]
    then
        varray=${commonVar}$(echo " " Eav{0,1,2,3,4,5} q3)
    elif [ $anaid == NUBAR1PIMINERvAGENIEOOB -o $anaid == NUBAR1PIMINERvAGENIEISO  -o $anaid == NUBAR1PIMINERvAGENIEISOw2 ]
    then
        varray=${commonVar}" pionEk piontheta pionmomentum"
    elif [ $anaid == MMECCQEMINERvAMEGENIEOOB ]
    then
        varray=${commonVar}$(echo q2qe muonpt mupz muonptVSmupz mupzVSmuonpt )
# ErecoilTIMES1E3VSmupzVSmuonpt 
#muonpt{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14} mupz{0,1,2,3,4,5,6,7,8,9,10,11,12} 
    else
        echo wrong anaid $anaid
        exit
    fi

    echo    
    echo $varray
    echo

    for var in $varray
    do

        echo var $var 
        echo

        root -b -l -q 'getStack.C("'${fin}'", "'${anaid}'","'${tag}'","'${var}'")'

    done
done

echo nfile $nfile

if [ $nfile != 0 ]
then
    cd outStack/${anaid}
    ls *.root
    hadd ${anaid}.root *.root > seemerge.log
fi

done


