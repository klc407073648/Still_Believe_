#build file path
jsoncpp_path="jsoncpp"
tinyxml_path="tinyxml"
log4cpp_path="log4cpp"
poco_path="poco"
cur_path=`pwd`

LOG_DIR=$cur_path/logPath
LOG_FILE=$LOG_DIR/build_comlib.log


function write_log()
{
  if [ ! -d ${LOG_DIR} ];then
	 mkdir -p ${LOG_DIR} 
  fi
  
  if [ ! -d ${LOG_FILE} ];then
	 touch  ${LOG_FILE} 
  fi
  
  fileCount=`ls ${LOG_DIR}/*.log |wc -l`
  
  if [ ${fileCount} -gt 10 ];then
     find ${LOG_DIR} -type f |xargs ls -tr | head -5 | xargs rm
  fi
  
  echo -n `date "+%Y-%m-%d %T "` >>${LOG_FILE}
  echo " $1" >>${LOG_FILE}
  
  return 0
}

write_log "build so file begin"

#生成jsoncpp库的so
write_log "build jsoncpp so begin"

cd $cur_path/$jsoncpp_path
tar -zxvf jsoncpp-src-0.5.0.tar.gz
cd ./jsoncpp-src-0.5.0
python /usr/bin/scons platform=linux-gcc
libname=`find ./libs -name "libjson*.so"`
#libPath=`echo ${libname%/*}`
#finalName=`echo ${libname##*/}`
cp $libname $cur_path/libjsoncpp.so

write_log "build jsoncpp so end"

write_log "cp jsoncpp include and so begin"
rm -rf $cur_path/../comlib/3part/jsoncpp/include/json/*.h
rm -rf $cur_path/../comlib/3part/jsoncpp/lib/*.so
cp -rf $cur_path/$jsoncpp_path/jsoncpp-src-0.5.0/include/json/*.h $cur_path/../comlib/3part/jsoncpp/include/json/
cp -rf $cur_path/libjsoncpp.so $cur_path/../comlib/3part/jsoncpp/lib/ 

if [ $? != 0 ];then 
	write_log "cp libjsoncpp.so fail , quit!"
	echo "cp libjsoncpp.so fail , quit!"
	exit 1
fi

write_log "cp jsoncpp include and so end"


#生成tinyxml库的so
write_log "build tinyxml so begin"

cd $cur_path/$tinyxml_path
unzip tinyxml_2_6_2.zip
cd ./tinyxml
cp ../Makefile ./Makefile
make
cp libtinyxml.so $cur_path/libtinyxml.so

write_log "build tinyxml so end"

write_log "cp jsoncpp include and so begin"
rm -rf $cur_path/../comlib/3part/tinyxml/include/*.h
rm -rf $cur_path/../comlib/3part/tinyxml/lib/*.so
cp -rf $cur_path/$tinyxml_path/tinyxml/*.h $cur_path/../comlib/3part/tinyxml/include/
cp -rf $cur_path/libtinyxml.so $cur_path/../comlib/3part/tinyxml/lib/ 

if [ $? != 0 ];then 
	write_log "cp libtinyxml.so fail , quit!"
	echo "cp libtinyxml.so fail , quit!"
	exit 1
fi

write_log "cp jsoncpp include and so end"

#生成log4cpp库的so
write_log "build log4cpp so begin"

cd $cur_path/$log4cpp_path
log4cpp_output="$cur_path/$log4cpp_path/output"
mkdir -p  $log4cpp_output
tar -zxvf log4cpp-1.1.3.tar.gz
cd ./log4cpp
./autogen.sh    #需要yum install libtool
./configure  --prefix=$log4cpp_output
#make
#make check
#make install
#不作check
make && make install

write_log "build log4cpp so end"

write_log "cp log4cpp include and so begin"
rm -rf $cur_path/../comlib/3part/log4cpp/include/*
rm -rf $cur_path/../comlib/3part/log4cpp/lib/*
cp -rf $log4cpp_output/include/* $cur_path/../comlib/3part/log4cpp/include
cp -rf $log4cpp_output/lib/liblog4cpp.so.5.0.6 $cur_path/../comlib/3part/log4cpp/lib

if [ $? != 0 ];then 
	write_log "cp liblog4cpp.so.5.0.6 fail , quit!"
	echo "cp liblog4cpp.so.5.0.6 fail , quit!"
	exit 1
fi

if [ -f $cur_path/../comlib/3part/log4cpp/lib/liblog4cpp.so.5.0.6 ];then
   cd  $cur_path/../comlib/3part/log4cpp/lib
   ln -s  liblog4cpp.so.5.0.6 liblog4cpp.so
   ln -s  liblog4cpp.so.5.0.6 liblog4cpp.so.5
   write_log "link log4cpp success"
fi

write_log "cp log4cpp include and so end"

#生成POCO库的so
write_log "build poco so begin"

cd $cur_path/$poco_path
poco_output="$cur_path/$poco_path/output"
mkdir -p  $poco_output
tar -zxvf poco-1.10.1-all.tar.gz
cd ./poco-1.10.1-all
./configure --config=Linux --no-tests --no-samples --minimal --prefix=$poco_output
#./configure --no-tests --no-samples --omit=Data/ODBC,Data/SQLite,Data/PostgreSQL,MongoDB,Redis --prefix=$poco_output
make && make install

write_log "build poco so end"

write_log "cp poco include and so begin"
rm -rf $cur_path/../comlib/3part/poco/include/*
rm -rf $cur_path/../comlib/3part/poco/lib/*
cp -rf $poco_output/include/* $cur_path/../comlib/3part/poco/include
cp -rf $poco_output/lib/*.so.71 $cur_path/../comlib/3part/poco/lib


cd  $cur_path/../comlib/3part/poco/lib

#${i%%.*}：删掉第一个.及其右边的字符串
for i in `ls`
do
	ln -s $i ${i%%.*}.so
done


write_log "cp poco include and so end"

write_log "build so file end"

write_log "clean generate file"
rm -rf $cur_path/$jsoncpp_path/jsoncpp-src-0.5.0
rm -rf $cur_path/$tinyxml_path/tinyxml
rm -rf $cur_path/$log4cpp_path/output
rm -rf $cur_path/$log4cpp_path/log4cpp
rm -rf $cur_path/$poco_path/output
rm -rf $cur_path/$poco_path/poco-1.10.1-all
rm -rf $cur_path/*.so

