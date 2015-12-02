enno sdk for mbed(LPC1768)
=========================


The following instructions describe the steps for connecting an mbed-enabled SmeshLink xbed(based on NXP LPC1768) device to Enno IoT platform.

----------

Requirements
-------------

 - Required hardware: mbed-enabled NXP LPC1768(SmeshLink xbed is recommend).
 - SSH client on your desktop computer, such as PuTTY, so you can remotely access the command line on the Freescale FRDM-K64F.

---------
Connect the device
-------------

 -   Connect the board to your network using an Ethernet cable. This step is required, as the sample depends on internet access.
 - Plug the device into your computer using a micro-USB cable. Be sure to attach the cable to the USB port.
 - Follow the [instructions on the mbed handbook](https://developer.mbed.org/handbook/SerialPC) to set up the serial connection with your device from your development machine. If you are on Windows, install the Windows serial port drivers located [here](https://developer.mbed.org/handbook/Windows-serial-configuration#1-download-the-mbed-windows-serial-port).


----------


Create mbed project and import the sample code
---------------

 - In your web browser, go to the mbed.org [developer site](https://developer.mbed.org/). If you haven't signed up, you will see an option to create a new account (it's free). Otherwise, log in with your account credentials. Then click on **Compiler** in the upper right-hand corner of the page. This should bring you to the Workspace Management interface.![enter image description here](https://lh3.googleusercontent.com/YVy0SjTVQ8ZgDGTOAbW-0S5UTDawidGs56IDMNV2iA=s0 "m1.PNG")
 - Make sure the hardware platform you're using appears in the upper right-hand corner of the window, or click the icon in the right-hand corner to select your hardware platform.
 - Click **Import** on the main menu. Then click the **Click here** to import from URL link next to the mbed globe logo.![enter image description here](https://lh3.googleusercontent.com/-ntsI_2uZwlI/Vl5lItWa4FI/AAAAAAAAACE/PjA8HqWr70A/s0/m2.PNG "m2.PNG")
 - In the popup window, enter the link for the sample code
 https://developer.mbed.org/users/weiarm2015/code/MQTT/![enter image description here](https://lh3.googleusercontent.com/-cc3kgGHyg-Q/Vl5lPyGGDxI/AAAAAAAAACQ/R6jbAu7qElU/s0/m3.PNG "m3.PNG")
 
 - You can see in the mbed compiler that importing this project imported various libraries. Some are provided and maintained by the Enno IoT team (enno_api.h, enno_api.c, enno_api_mqtt_wrapper.c, CplusplusToC.cpp, enno_error.h, enno_config.h,enno_log.h), while others are third party libraries available in the mbed libraries catalog.![enter image description here](https://lh3.googleusercontent.com/-t4EeFiJAX-4/Vl5lpx2iTQI/AAAAAAAAACY/la26eL9Gczo/s0/m5.PNG "m5.PNG")
 


----------
Build and run the program
---------------

 - Click Compile to build the program. You can safely ignore any warnings, but if the build generates errors, fix them before proceeding.![enter image description here](https://lh3.googleusercontent.com/-isao5DKd3Kg/Vl5lv55axkI/AAAAAAAAACg/1ui8Su85kU4/s0/m4.PNG "m4.PNG")
 - If the build is successful, a .bin file with the name of your project is generated. Copy the .bin file to the device. 
 - The program starts executing. You may have to reset the board if the program does not start automatically when you connect.
 


----------
Verify
-------

 - Login on Enno platform
 http://sitewhere.chinacloudapp.cn:8080/sitewhere/admin/
 
 - An mbed device should already been registered:
 - ![enter image description here](https://lh3.googleusercontent.com/-w-oU8kkN0Ck/Vl5seHAMumI/AAAAAAAAAD8/63d-NMOXg-s/s0/m10.PNG "m10.PNG")
 - Drill down the device link, you should see the location data is continous updating:
 ![enter image description here](https://lh3.googleusercontent.com/jrujgqxe4UqJP8eHB_PgxHMqxXvL9-nFUin4ryvRcQ=s0 "m11.PNG")
 - Now you already have your device successfully connected.

 

