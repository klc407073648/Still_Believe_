#contain sh
config_file=comlib_config

source ./common.sh

cur_path=`pwd`

#defalut print logDebug
function writeLogFileAndEcho
{
   if [ "$2"x = ""x ];then
       write_log "$1"  && logDebug "$1"
   else
       write_log "$1"  && "$2" "$1"
   fi
}

function parseConfigFile
{
   writeLogFileAndEcho "parseConfigFile begin" 

   logInfo "parse var from $config_file"

   jsoncpp_tar_name=`cat $config_file |grep "jsoncpp_tar_name=" |cut -f2 -d'='`
   log4cpp_tar_name=`cat $config_file |grep "log4cpp_tar_name=" |cut -f2 -d'='`
   
   tinyxml_zip_name=`cat $config_file |grep "tinyxml_zip_name=" |cut -f2 -d'='`
   zeromq_zip_name=`cat $config_file |grep "zeromq_zip_name=" |cut -f2 -d'='`
   cppzmq_zip_name=`cat $config_file |grep "cppzmq_zip_name=" |cut -f2 -d'='`

   jsoncpp_path=`cat $config_file |grep "jsoncpp_path=" |cut -f2 -d'='`
   tinyxml_path=`cat $config_file |grep "tinyxml_path=" |cut -f2 -d'='`
   log4cpp_path=`cat $config_file |grep "log4cpp_path=" |cut -f2 -d'='`
   zeromq_path=`cat $config_file |grep "zeromq_path=" |cut -f2 -d'='`

   build_list=`cat $config_file |grep "build_list=" |cut -f2 -d'='`

   echo "jsoncpp_tar_name=$jsoncpp_tar_name"
   echo "log4cpp_tar_name=$log4cpp_tar_name"
   
   echo "tinyxml_zip_name=$tinyxml_zip_name"
   echo "zeromq_zip_name=$zeromq_zip_name"
   echo "cppzmq_zip_name=$cppzmq_zip_name"

   echo "jsoncpp_path=$jsoncpp_path"
   echo "tinyxml_path=$tinyxml_path"
   echo "log4cpp_path=$log4cpp_path"
   echo "zeromq_path=$zeromq_path"

   echo "build_list=$build_list"

   writeLogFileAndEcho "parseConfigFile end"
}

function copySoAndHead
{
   if [ "$build_name"x != "zeromq"x ];then
      echo "${build_name} .h and .so deal begin"
      rm -rf $cur_path/../comlib/3part/$build_name/include/*
      rm -rf $cur_path/../comlib/3part/$build_name/lib/*
      cp -rf $build_include_path/* $cur_path/../comlib/3part/$build_name/include
      cp -rf $build_so_path/*${build_name}* $cur_path/../comlib/3part/$build_name/lib
      echo "${build_name} .h and .so deal end"
   fi

   if [ $? != 0 ];then
      write_log "cp $build_name file fail , quit!" && logError "cp $build_name file fail , quit!"
      exit 1
   fi  
}

function buildComlib
{
   writeLogFileAndEcho "buildComlib begin" 

   for build_name in $build_list  
   do 
      writeLogFileAndEcho "build $build_name begin"
      cd $cur_path
      
      if [ "$build_name"x = "jsoncpp"x ];then
         cd $jsoncpp_path
         tar -zxvf ${jsoncpp_tar_name}
         prefixJson=${jsoncpp_tar_name%%.tar.gz}
         cd ./$prefixJson

         python /usr/bin/scons platform=linux-gcc 
         libname=`find ./libs -name "libjson*.so"`

         mkdir -p $cur_path/jsoncpp_lib
         cp $libname $cur_path/jsoncpp_lib/libjsoncpp.so

         build_include_path=$cur_path/$jsoncpp_path/$prefixJson/include
         build_so_path=$cur_path/jsoncpp_lib

      elif [ "$build_name"x = "log4cpp"x ];then
         cd $log4cpp_path
         log4cpp_output="$cur_path/$log4cpp_path/output"
         mkdir -p  $log4cpp_output
         tar -zxvf $log4cpp_tar_name
         cd ./$build_name
         ./autogen.sh    #需要yum install libtool
         ./configure  --prefix=$log4cpp_output

         make -j4  && make install

         build_include_path=$log4cpp_output/include
         build_so_path=$log4cpp_output/lib
      elif [ "$build_name"x = "tinyxml"x ];then
         cd $tinyxml_path
         unzip $tinyxml_zip_name
         cd ./$build_name
         cp ../Makefile ./Makefile
         make
         cp libtinyxml.so $cur_path/libtinyxml.so

         build_include_path=$cur_path/$tinyxml_path/$build_name
         build_so_path=$cur_path
      elif [ "$build_name"x = "zeromq"x ];then
         cd $zeromq_path
         unzip $zeromq_zip_name
         prefixLibzmq=${zeromq_zip_name%%.zip}
         cd ./$prefixLibzmq/
         mkdir build && cd build
         cmake ..
         sudo make -j4 install

         cd $cur_path/$zeromq_path
         unzip $cppzmq_zip_name

         prefixCppzmq=${cppzmq_zip_name%%.zip}
         cd ./$prefixCppzmq/
         mkdir build && cd build
         cmake -DCPPZMQ_BUILD_TESTS=OFF ..
         sudo make -j4 install
      fi

      if [ "$build_name"x != "zeromq"x ];then
         writeLogFileAndEcho "${build_name} .h and .so deal begin"
         rm -rf $cur_path/../comlib/3part/$build_name/include/*
         rm -rf $cur_path/../comlib/3part/$build_name/lib/*
         cp -rf $build_include_path/* $cur_path/../comlib/3part/$build_name/include
         cp -rf $build_so_path/*${build_name}* $cur_path/../comlib/3part/$build_name/lib
         writeLogFileAndEcho "${build_name} .h and .so deal end"
      fi

      if [ "$build_name"x = "log4cpp"x ];then
          if [ -f $cur_path/../comlib/3part/log4cpp/lib/liblog4cpp.so.5.0.6 ];then
             cd  $cur_path/../comlib/3part/log4cpp/lib
             ln -s  liblog4cpp.so.5.0.6 liblog4cpp.so
             ln -s  liblog4cpp.so.5.0.6 liblog4cpp.so.5
             write_log "link log4cpp success"
          fi
      fi

      if [ $? != 0 ];then
         writeLogFileAndEcho "cp $build_name file fail , quit!" "logError"
         exit 1
      fi

      writeLogFileAndEcho "build $build_name end"
      
   done  

   writeLogFileAndEcho "buildComlib end"
}

function clearBuildPath
{
   writeLogFileAndEcho "clearBuildPath begin"

   rm -rf $cur_path/$jsoncpp_path/${jsoncpp_tar_name%%.tar.gz}
   rm -rf $cur_path/$tinyxml_path/tinyxml
   rm -rf $cur_path/$log4cpp_path/output
   rm -rf $cur_path/$log4cpp_path/log4cpp
   rm -rf $cur_path/$poco_path/output
   rm -rf $cur_path/$poco_path/poco-1.10.1-all
   rm -rf $cur_path/$zeromq_path/${zeromq_zip_name%%.zip}
   rm -rf $cur_path/$zeromq_path/${cppzmq_zip_name%%.zip}
   rm -rf $cur_path/*.so

   writeLogFileAndEcho "clearBuildPath end"
}

function MAIN
{
   writeLogFileAndEcho "MAIN begin"
   parseConfigFile
   clearBuildPath
   buildComlib
   clearBuildPath
   writeLogFileAndEcho "MAIN end" 
}

MAIN
