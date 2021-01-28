curPath=`pwd`
echo $curPath
cd $curPath/build_comlib
dos2unix *.sh
dos2unix *config
chmod 777 *.sh
./buildComlib.sh

if [ $? != 0 ];then 
	echo "buildComlib.sh fail , quit!"
	exit 1
fi

cd $curPath/build
rm -rf ./*
cmake  ..
make 

#rm -rf $curPath/build/*
