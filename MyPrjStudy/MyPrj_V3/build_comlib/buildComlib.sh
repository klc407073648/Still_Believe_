#build file path
jsoncpp_path="jsoncpp"
tinyxml_path="tinyxml"
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

#build jsoncpp so
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

write_log "cp jsoncpp include and so end"


#build tinyxml so
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

write_log "cp jsoncpp include and so end"

write_log "build so file end"


write_log "clean generate file"
rm -rf $cur_path/$jsoncpp_path/jsoncpp-src-0.5.0
rm -rf $cur_path/$tinyxml_path/tinyxml
rm -rf $cur_path/*.so

