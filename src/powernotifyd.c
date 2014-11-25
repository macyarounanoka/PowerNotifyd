#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>   
#include <sys/stat.h>
 
#include <mach/mach_port.h>
#include <mach/mach_interface.h>
#include <mach/mach_init.h>
 
#include <IOKit/pwr_mgt/IOPMLib.h>
#include <IOKit/IOMessage.h>

#include "../config.h"
 
io_connect_t  root_port;
IONotificationPortRef  notifyPortRef;
io_object_t            notifierObject;
void*                  refCon=NULL;


int LOG_LEVEL = LOG_NOTICE;
//int LOG_LEVEL = LOG_INFO;


extern void callscripts();

void
MySleepCallBack( void * refCon, io_service_t service, natural_t messageType, void * messageArgument )
{
    switch ( messageType )
    {
 
        case kIOMessageCanSystemSleep:
            IOAllowPowerChange( root_port, (long)messageArgument );
            break;
 
        case kIOMessageSystemWillSleep:
            IOAllowPowerChange( root_port, (long)messageArgument );
            break;
 
        case kIOMessageSystemWillPowerOn:
            break;
 
        case kIOMessageSystemHasPoweredOn:
            break;
 
        default:
            break;
 
    }
    callscripts( messageType);
}



void callscripts( int nType){
    
    char fullpathCommand[1024];
    char *scriptFile = NULL;
    switch( nType ){
        case kIOMessageCanSystemSleep:
            scriptFile = SCRIPT_FILE_CANSYSTEMSLEEP;
            break;
        case kIOMessageSystemWillSleep:
            scriptFile = SCRIPT_FILE_SYSTEMWILLSLEEP;
            break;
        case kIOMessageSystemWillPowerOn:
            scriptFile = SCRIPT_FILE_SYSTEMWILLPOWERON;
            break;
        case kIOMessageSystemHasPoweredOn:
            scriptFile = SCRIPT_FILE_SYSTEMHASPOWEREDON;
            break;
        default:break;
    }
    if( scriptFile == NULL ){ return ;}
    sprintf( &fullpathCommand[0], "%s",scriptFile);
    struct stat scriptStat;
    if( stat( fullpathCommand, &scriptStat) == -1 ){
        syslog( LOG_LEVEL, "not found %s", fullpathCommand);
        return ; // NO FILE
    }
    if( (scriptStat.st_mode & S_IFREG) == S_IFREG &&
        (scriptStat.st_mode & S_IXUSR) == S_IXUSR &&
       (scriptStat.st_mode & S_IRUSR) == S_IRUSR ){
        ;// OK File
    }
    else { return ;// Not access File
        syslog( LOG_LEVEL, "can't execute. Need \"chmod 755 %s\"", fullpathCommand);
    }
    
    
    FILE * fp ;
    char buf[1024];
    if( (fp=popen(fullpathCommand, "r"))== NULL){
        syslog( LOG_LEVEL, "can't execute. system was not support file? %s", fullpathCommand);
        return ;// Cant Open Commad
    }
    syslog(LOG_LEVEL, "run %s", fullpathCommand);
    while(feof(fp)==false){
        memset( buf, 0, 1024);
        fgets(buf,1023,fp);
        if( strlen( buf ) != 0){
            syslog( LOG_LEVEL, "->%s", &buf[0]);
        }
    }
    pclose(fp);
}


int main( int argc, char **argv )
{
    syslog(LOG_LEVEL, "start");
 
    root_port = IORegisterForSystemPower( refCon, &notifyPortRef, MySleepCallBack, &notifierObject );
    if ( root_port == 0 )
    {
        syslog(LOG_ERR,"IORegisterForSystemPower failed\n");
        return 1;
    }
 
    CFRunLoopAddSource( CFRunLoopGetCurrent(),
            IONotificationPortGetRunLoopSource(notifyPortRef), kCFRunLoopCommonModes );
 
    CFRunLoopRun();
    syslog(LOG_LEVEL, "end");
    return (0);
}


