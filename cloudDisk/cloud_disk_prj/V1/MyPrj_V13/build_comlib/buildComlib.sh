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

write_log "build so file end"

write_log "clean generate file"

rm -rf $cur_path/$log4cpp_path/output
rm -rf $cur_path/$log4cpp_path/log4cpp
rm -rf $cur_path/*.so

