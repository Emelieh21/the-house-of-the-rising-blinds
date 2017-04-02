# The House of The Rising Blinds

Today we finally getting started with our next SmartHome project on the list: **The Blind Hack**. My personal dream hack: install a luminosity sensor close to the living room window that communicates with a relay connected to the blinds in the bed room, to make the blinds go up automatically when the sun starts shining (or on a cloudy day in Berlin: it gets light outside).

![image](https://github.com/Emelieh21/the-house-of-the-rising-blinds/blob/master/assets/risingblinds.jpg)

## STEP 1: Resetting the OrangePi

_Unfortunately_, we were faced with a tiny bump in the road: we corrupted our OrangePi (probably by unplugging it unexpectedly). So the first step is a step back: setting up the OrangePi again.

To reset the OrangePi we reformatted the 32 GB mini SD card (which costed 3 euro - so no wonder it got corrupted a bit)  and burned the armbian image again to the card. We needed to do the usual setup again (change root password, set a user and a user password.) In the process we found out some commands that might be handy in the future:

* Discorvery #1: shutting down the OrangePi via SSH:

  ``sudo /sbin/shutdown -h now``

  After running this command we saw the OrangePi green light started flickering like crazy. At some point it stopped and the HDMI connected screen went off.  The light and the screen stayed off. The command seems to work well.


* The command we always need, but always forget to write down:

  ```ssh-keygen -R "192.168.178.64"```


* Before the big OrangePi we transferred our necessary files to Jaime's pc with scp:

  ```scp -r ~/welcome-home-sonos jaime@192.168.178.22:~/```

For the rest we followed the steps for installing NodeJS, NPM and the node-sonos-http-api that we documented in our [earlier repository](https://github.com/chronoclast/sensational-sonos-sensor-synchronization/tree/master/orange-pi-setup).

## STEP 2: Connect the Luminosity Sensor to the relayr Cloud

First of all: follow [this tutorial](https://github.com/relayr/ESP8266_Arduino/) to get your pc ready for Arduino action.

#### Sensor Basics:

VCC:	red = voltage (almost always 5v or 3.3v) (DC = CC)

GND/G:	black = ground

NC:		not connected (white cable = useless for almost all digital sensors)

SIG: 	the output of the sensor (signal)

**TIP FROM JAIME: Disconnect the power of the board before plugging the sensor!**

In the Arduino IDE, do not forget to set up the `Tools` > `Port` to the USB port that your board is connected too. 

Connect the luminosity sensor to the WeMos. We use an analog luminosity sensor (luminance sensor v1.0). The signal cable (in our case the yellow one) if connected to the **A0** pin on the WeMos:

![image2](https://github.com/Emelieh21/the-house-of-the-rising-blinds/blob/master/assets/wemos_sensor_3jpg.jpg)

To see the values from the luminosity sensor, you can flash the following Arduino code to the WeMos:

```C
#define LUMINOSITY_SENSOR_PIN A0  // choose the input pin (for PIR sensor)

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // declare LED as output
  pinMode(LUMINOSITY_SENSOR_PIN, INPUT);     // declare sensor as input
  Serial.begin(9600);
}
 
void loop(){
  Serial.println(analogRead(LUMINOSITY_SENSOR_PIN));
  delay(1000);
}
```

To check the printed values, click on the _Serial Monitor_ in the Arduino IDE (little icon in the right upper corner).

The outside daylight - whether there is full sun or a cloud, gives us always the **maximum value: 1024**. Inside the house during the day we get a value of around 400/450.

Great! Our sensor is sending us the values we need. Now let's connect it to the relayr cloud. For this we need to set up a device in the cloud. A detailed description how to set up a device in the relayr cloud, you can find [here](https://github.com/Emelieh21/brand-new-kettle-hack/blob/master/setup_devices_relayr_cloud.md). Copy paste your credentials to the [luminosity_relayr_cloud.ino](D:\SundayFunday\luminosity_relayr_cloud.ino) file and flash it to the WeMos.

Now your device in the relayr cloud should start receiving values:

![image2](https://github.com/Emelieh21/the-house-of-the-rising-blinds/blob/master/assets/cloud.jpg)

## STEP 3: Make the Light Sensor Interact with the Sonos

As described in [one of our earlier respositories](https://github.com/chronoclast/sensational-sonos-sensor-synchronization), we have a server running in the house that makes it possible to control the sonos via an API. In this next step we make the values of the luminosity sensor control the sonos. In this first simple script, we make the sonos stop playing every time it gets dark (luminosity < 50). If it gets light again (luminosity > 50), the sonos will start to play again.

For this script you will need the requests and the mqtt library for NodeJs. These steps are assuming you have NodeJS and npm installed on your pc.

```$ npm install request```

```$ npm install mqtt```

The only thing you need to do is put the credentials of your relayr device in the 03-welcome-home-sonos_sunshine.js script. Now you can run the script with:

```$ node 03-welcome-home_sunshine.js```

And play with your sonos by covering the light sensor and uncovering it.



------------ TO BE CONTINUED ------------

Up next: 

* replace the OrangePi to the hallway
* run the scripts on the OrangePi with forever
* change the 03-welcome-home_sunshine.js script so it will make the sonos go on in the morning when we are suppossed to wake up. 
