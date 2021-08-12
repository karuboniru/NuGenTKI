date

fin=$1

readlink -f $fin

tag=$2_$(basename $fin | awk -F\. '{print $1}')

anamode=$3

ntostop=$4

echo fin ${fin}
echo tag ${tag}
echo anamode ${anamode}
echo ntostop ${ntostop}

./anaGenerator ${tag} ${fin}  ${anamode} ${ntostop}

date

exit

root -b -l <<EOF
.L anaGenerator.cxx+
anaGenerator("${tag}", "${fin}", ${anamode})
EOF
