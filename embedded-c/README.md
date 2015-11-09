# Microsoft Enno Device SDK

This repository contains IoT device SDK for Enno platform.

Directory structure Current SDK Directory Layout :
```
SOURCE_ROOT
|── src(Source files of the enno IoT device SDK)
|── protocol_lib(communication protocol library, current Paho MQTT only)
|── serialization_lib(serialization library, current nanopb only)
|──samples(sample application project for different platforms and protocols)
|──docs(Developer guide & API documentation)

``` 
  
Only 3 steps are needed to run the basic sample application:

## Build 

To build the project and generate the executable file, you should input the following commands:
```
$cd YOUR_SOURCE_ROOT
$mkdir build && cd build
$cmake ..
$make
```
This will automatically generate all the application in the samples directory for you.
## Run
```
$cd samples/simple-pubsub
$./simple-pubsub
```

The output  should be:
```
Connecting to enno server 139.217.10.99:1883...connected.
Subscribing system topic...subscribed.
Send device registeration...sent.
Registered new device
Location updated.
Location updated.
Location updated.
Location updated.

```

##Verify

Login to [Enno platform](http://http://sitewhere.chinacloudapp.cn:8080/sitewhere/admin/devices/list.html).

An Arduino device should already been registered:
 
![](http://i.imgur.com/iglx2Ax.png)

Drill down the device link, you should see the location data is continous updating:

![](http://i.imgur.com/AaHZhlV.png)
    
Now you already have your device successfully connected.
