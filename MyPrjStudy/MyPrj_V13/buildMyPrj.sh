curPath=`pwd`
echo $curPath
cd $curPath/build_comlib
chmod 777 buildComlib.sh
./buildComlib.sh

if [ $? != 0 ];then 
	echo "buildComlib.sh fail , quit!"
	exit 1
fi

cd $curPath/build
rm -rf ./*
cmake ..  &&  make

rm -rf $curPath/build/*

cd $curPath/shell
chmod 777 *.sh
./start.sh
