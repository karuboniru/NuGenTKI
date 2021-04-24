export PIPLUS1GEV="list/Filelist_GEANT4_v0test.txt"

ntostop=
#10000000

tag=GEANT4_test0_

####################################

mkexe.sh anaGenerator -I${NUGENTKI}/include

#exit

#GFS 9 : 0PI
opt=${tag}PIPUS1GEV;          nohup ./doAna.sh $PIPLUS1GEV         ${opt} 0 ${ntostop} > see${opt}.log &

#exit
