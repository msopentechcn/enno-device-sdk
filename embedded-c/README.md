# enno-device
Microsoft Enno IoT SDK

This repository contains IoT device SDK, which enables you connect client devices to Azure IoT Hub.

Directory structure Current SDK Directory Layout :

|--enno_src(Source files of the enno IoT device SDK)
|--enno_client(Client library)
|--enno_serialization(enno serialization library, base on nano pb)
|--samples(makefiles to build sample)
|--docs(Developer guide & API documentation)

The fast way to leanr about the API is to run the sample:
The sample shows a simple example to subscribe a topic, and then publish a few message(including registeing the device,
  publishing an Alert message and so on) to it. The default message to publish is "Alert", if you wanna test the other
  message(for example "Location"), you should set up the info_t in as 'L': 
       enno_publish(PublishParams *pParams, char Info_t ,EnnoParams *Params_enno);
       info_t may set up as :
         'R': which means to publish the Registeration
         'A': which means to publish the Alert
         'M': which means to publish the Measurement
         'L': which means to publish the Location
  
You need two steps to run the sample:
  ## Build 
   To build the project and generate the executable file, you should input the following commands:
     cd ../samples/linux_platform/mqtt
     sh ./build1.sh
     then will generate the executable file---subpub


  ## run
    To run the sample, you only need input the following command:
     ./subpub
    
    


