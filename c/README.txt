Quick Installation Guide - OpenIoT agent for Debian
=====================================================

Install required library:
$sudo apt-get update
$sudo apt-get install uuid-dev openssl libssl-dev

Install Qpid-proton-c 0.9.1:
https://git-wip-us.apache.org/repos/asf?p=qpid-proton.git;a=blob_plain;f=INSTALL.md;hb=0.9.1

Compile OpenIoT agent:
$makedir build
$cd build
$cmake ..
$make V=s

Test openiot-agent command line interface:
$openiot-agent-cli -h

Sample shell:
openiot-agent: autostart script
openiot-agent.sh: sample shell to call openiot-agent-cli