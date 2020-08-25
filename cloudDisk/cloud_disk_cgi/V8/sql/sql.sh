#!/bin/bash
mysql -h localhost  -u root -P 3306 -pklczxas789 -Dcloud_disk<./cloudDisk.sql  > ./sql.log
