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
cmake -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_BUILD_PROJECT_OPTION=all ..
make -j8

#rm -rf $curPath/build/*
