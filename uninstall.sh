#!/bin/sh

program_name=`cat install.sh|sed -ne '/program_name=\(\w*\)/p' | sed -e 's/program_name=\(\w*\)/\1/'`
install_plist_dir=~/Library/LaunchAgents


launched=`launchctl list | grep local.$program_name`

strlen=${#launched}
if [ $strlen != 0 ]; then
  echo "unload $install_plist_dir/local.$program_name.plist"
 launchctl unload $install_plist_dir/local.$program_name.plist
fi


if [ -f $install_plist_dir/local.$program_name.plist ]; then
   echo "delete $install_plist_dir/local.$program_name.plist"
   rm -f $install_plist_dir/local.$program_name.plist
fi

echo "uninstall.sh done."

