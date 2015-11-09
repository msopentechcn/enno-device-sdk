#!/bin/sh

Macaddr=`cat /sys/class/net/eth0/address | tr a-z A-Z`
FirmwareVersion=`cat /etc/openwrt_release | awk -F "=" '/DISTRIB_DESCRIPTION/{print substr($2,0)}'`
FlashAvailable=`df | awk '/rootfs/{print substr($4,0)}'`
FlashUsage=`df | awk '/rootfs/{print substr($5,0)}'`
Cpufreq=580HZ
CpuModel=`cat /proc/cpuinfo | awk '/model/{print substr($4,0) " " substr($5,0) " " substr($6,0) }'`
SystemType=`cat /proc/cpuinfo | awk '/system/{print substr($4,0) " " substr($5,0) " " substr($6,0)}'`
Machine=`cat /proc/cpuinfo | awk '/machine/{print substr($3,0)}'`

echo "MacAddr:         "$Macaddr 
echo "FirmwareVersion: "$FirmwareVersion
echo "FlashAvailable:  "$FlashAvailable
echo "FlashUsage:      "$FlashUsage
echo "Cpufreq:         "$Cpufreq 
echo "CpuModel:        "$CpuModel   
echo "SystemType:      "$SystemType   
echo "Machine:         "$Machine
 
Outbound='amqps://{SAS_KEY}:{SAS_TOKEN}@{NAMESPACE}.servicebus.chinacloudapi.cn/{ENTITY_PATH}'
SpecId='{SPEC_ID}'
HardwareId=$Macaddr

/usr/bin/openiot-agent.bin -o $Outbound -i $HardwareId  -s $SpecId -r

while true
do
    MemTotal=`cat /proc/meminfo | awk '/MemTotal/{print substr($2,0)}'`
    MemFree=`cat /proc/meminfo | awk '/MemFree/{print substr($2,0)}'`
    let M=$MemTotal-$MemFree
    let MemUsage=$M*100/$MemTotal
    CpuUsage=`top -b -n1 | grep "CPU:" | grep -v grep | awk '{print $2 + $4}'`
    DiskUsage=`df -h | awk '{if ($6 == "/") { print $5 }}' | head -1 | cut -d'%' -f1`

    /usr/bin/openiot-agent.bin -o $Outbound -i $HardwareId  -s $SpecId -m cpu.utils:$CpuUsage,mem.utils:$MemUsage,disk.utils:$DiskUsage

    sleep 5

    /usr/bin/openiot-agent.bin -o $Outbound -i $HardwareId  -s $SpecId -a "health.keepalive:device is alive"
    
    sleep 5
     
done

