# Push data to OVH metrics timeseries from ESP8266

This example shows how to push data on OVH Metrics plateform from an arduino **Wemos**. **Wemos** is an implementation of the wifi module **ESP8266**. **Wemos** is programmable via Arduino IDE

(Whole tutorial here: <https://github.com/landru29/ovh_metrics_wemos/tree/master/tutorial>)

![Wemos](https://github.com/landru29/ovh_metrics_wemos/raw/master/wemos.png "Wemos D1 R2 mini")

## Prerequisite

* Have a WEMOS D1 Mini
* Have a sane installation of Arduino IDE
* Have **ESP8266** toolkit installed (<https://github.com/esp8266/Arduino>); see below for the installation part

## Install ESP8266 toolkit

* Install the current upstream Arduino IDE at the 1.8.7 level or later. The current version is on the [Arduino website](https://www.arduino.cc/en/main/software).
* Start Arduino and open the Preferences window.
* Enter ```https://arduino.esp8266.com/stable/package_esp8266com_index.json``` into the *Additional Board Manager URLs* field. You can add multiple URLs, separating them with commas.
* Open Boards Manager from Tools > Board menu and install *esp8266* platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).

## Hardware

* Wemos D1 Mini
* Wifi network (with WPS function)

## config.h

The file shold contain the following definition:

```c
#define TOKEN "put the 'write' token in graphana topic in the manager"
#define HOST "the host of your metrics plateform (ie 'opentsdb.gra1.metrics.ovh.net')"
#define FINGERPRINT "XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX"
```

* `TOKEN` is the write token you created in the OVH manager (Cloud > metrics): <https://www.ovh.com/manager/cloud/#/>.
* `HOST` is the host of the opentsdb plateform provided by OVH
* `FINGERPRINT` is the taken from the certificate of the server; see below

## Fingerprint

The fingerprint is the SHA1 of the server certificate. To get it, just run the following command in a linux environment (this is an example for the host `opentsdb.gra1.metrics.ovh.net`):

```bash
HOST=opentsdb.gra1.metrics.ovh.net; echo | openssl s_client -showcerts -servername ${HOST} -connect ${HOST}:443 2>/dev/null | openssl x509 -noout -fingerprint -sha1 -inform pem | sed -e "s/.*=//g" | sed -e "s/\:/ /g"
```

## Arduino

* Create a file `config.h` based on `config.h.sample`
* Select the correct board `LOLIN(WEMOS) D1 R2 & mini`
* Compile program

## Running

Before running for the first time, you must push on the **WPS** button of your internet modem. then you avec about 30 seconds to power you **Wemos**. The program try to negociate the connection with the modem.

For the other runs, both **Wemos** and your modem *remember* they are friends. You do not need to push WPS button.

Display the **Serial Monitor** from the ardiuno IDE.

If the connection succeed, it will display the IP address of the **Wemos**

Then to program will send the JSON to the OVH metrics plateform

You can follow the data by configuring you grafana (access from the OVH manager <https://www.ovh.com/manager/cloud/#/>)
