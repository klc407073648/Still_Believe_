curPath=`pwd`
echo $curPath
cd $curPath/build_comlib
chmod 777 buildComlib.sh
./buildComlib.sh

cd $curPath/build
rm -rf ./*
cmake ..  &&  make

rm -rf $curPath/build/*
