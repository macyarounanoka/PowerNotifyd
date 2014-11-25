#!/bin/sh

program_name=PowerNotifyd
install_plist_dir=~/Library/LaunchAgents
current_dir=`pwd`
path_to_scripts=$current_dir/scripts
path_to_bin=$current_dir/bin/$program_name
shCommandsKey=(CANSYSTEMSLEEP SYSTEMWILLSLEEP SYSTEMWILLPOWERON SYSTEMHASPOWEREDON)
shCommandsValue=(CanSystemSleep SystemWillSleep SystemWillPowerOn SystemHasPoweredOn)

if [ -f ./config.h ]; then
 rm -f ./config.h
 touch ./config.h
fi
iValue=0
for cmdkey in "${shCommandsKey[@]}"
do
 SCRIPT_PATH=$path_to_scripts/${shCommandsValue[$iValue]}
 echo "#define SCRIPT_FILE_${cmdkey} \"$SCRIPT_PATH\"" >> ./config.h
 iValue=$iValue+1
 chmod +x $SCRIPT_PATH
done

if [ ! -f ./bin/$program_name ]; then
  rm -f ./bin/$program_name
fi

./compile.sh $program_name

if [ ! -f ./bin/$program_name ]; then
  echo "error Need compile. please compile.sh"
  exit 1
fi



if [ -f ./local.$program_name.plist ]; then
  rm -f ./local.$program_name.plist 
fi

sed -e 's|PATH_TO_POWERNOTIFYD|'$path_to_bin'|' ./plist.template | sed -e 's/PROGRAM_LABEL/local.'$program_name'/' > ./local.$program_name.plist


if [ ! -d $install_plist_dir ]; then
   echo "error:not support osx version "
   exit 1
fi

launched=`launchctl list | grep local.$program_name`

strlen=${#launched}
if [ $strlen != 0 ]; then
 launchctl unload $install_plist_dir/local.$program_name.plist
fi


cp -p ./local.$program_name.plist $install_plist_dir/.

launched=`launchctl list | grep local.$program_name`
strlen=${#launched}
if [ $strlen = 0 ]; then
 launchctl load $install_plist_dir/local.$program_name.plist
fi


