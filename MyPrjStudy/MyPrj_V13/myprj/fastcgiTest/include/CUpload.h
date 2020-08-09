/**
 * @file CUpload.h
 * @brief 上传图片类
 * @author klc
 * @date 2020-08-09
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_CUPLOAD_H__
#define __STIBEL_CUPLOAD_H__

#include "Request.h"
#include "MySQL.h"
#include "Util.h"
#include "LoggerManager.h"
using namespace minilog;

class Upload
{

public:
        Response upload(Request req);

private:
        DECLARE_STATIC_LOGGERMANAGER();

};

#endif //__STIBEL_CUPLOAD_H__
