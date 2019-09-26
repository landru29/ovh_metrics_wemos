# How to

Here is, step-by-step, the complete documentation to make the project work.

## The hardware

The aim of the project is to plug a sensor on an Arduino that will send data to OVH Metric Data Platform via the network. Do you know **ESP8266** ? it's a low-cost Wi-Fi microchip with full TCP/IP stack and microcontroller capability. Yes, low-cost : less than 2€

![buy it](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/buy-esp8266.png)

### ESP8266 Fonctional Diagram

![fonctional diagram](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/esp8266.png)

### Implementation Wemos

ESP8266 is quite not easy to use alone:

* Must be powered in 3.3V (not much, il would burn)
* No USB
* ...

That's why, it is better to use a solution that implement ESP8266. Here is the Wemos !

![wemos](https://github.com/landru29/ovh_metrics_wemos/raw/master/wemos.png)

* Powered in 5V (via USB or a dediated pin
* USB for serial communication
* Can be programmed via USB
* **Costs less than 3€**

## Order a Metrics Data Platform

Go to OVH Metrics Data Platform website: <https://www.ovh.com/fr/data-platforms/metrics/>

![step00](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/step00.png)

Click on the free trial

![step01](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/step01.png)

Proceed ... you are asked to connect to your account. If you do not have one, juste create it.

![step02](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/step02.png)

## Prepare the configuration

Copy file `config.h.sample` into `config.h`. We will configure the items below...

### Get Token from your Metrics Data Plateform

Now, go and connect to the OVH Cloud Manager: <https://www.ovh.com/manager/cloud/#/>

On the left-hand panel, you should have **Metrics** and a new service inside

![step03](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/step03.png)

In the tab **Tokens** you can copy the write token and modify your `config.h`

![step04](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/step04.png)

```cpp
#define TOKEN "xxxxxxxxxx"
```

### Get the host of your server

Your are still in the OVH Cloud Manager: <https://www.ovh.com/manager/cloud/#/>, on your **Metrics** service.

In the tab **Platforms**, you can copy the opentsdb host. Drop `https://` and modify your `config.h`

![step05](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/step05.png)

```cpp
#define HOST "opentsdb.xxxxxx"
```

### Get the fingerprint of the certificate

You need the host you just grab from the **Platforms** tab;

#### Linux users

Just play this little script:

```bash
HOST=opentsdb.gra1.metrics.ovh.net; echo | openssl s_client -showcerts -servername ${HOST} -connect ${HOST}:443 2>/dev/null | openssl x509 -noout -fingerprint -sha1 -inform pem | sed -e "s/.*=//g" | sed -e "s/\:/ /g"
```

copy the result in your `config.h`

```cpp
#define FINGERPRINT "XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX"
```

#### Mac users

Just play this little script:

```bash
HOST=opentsdb.gra1.metrics.ovh.net; echo | openssl s_client -showcerts -servername ${HOST} -connect ${HOST}:443 2>/dev/null | openssl x509 -noout -fingerprint -sha1 -inform pem | sed -e "s/.*=//g" | sed -e "s/\:/ /g"
```

copy the result in your `config.h`

```cpp
#define FINGERPRINT "XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX"
```

#### Windows users

Sorry, **I don't know**

## Prepare your Arduino environment

Install the current upstream Arduino IDE at the 1.8.7 level or later. The current version is on the [Arduino website](https://www.arduino.cc/en/main/software).

Start Arduino and open the Preferences window.

Enter ```https://arduino.esp8266.com/stable/package_esp8266com_index.json``` into the *Additional Board Manager URLs* field. You can add multiple URLs, separating them with commas.

![step06](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/step06.png)

Open Boards Manager from Tools > Board menu and install *esp8266* platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).

![step07](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/step07.png)

Now Select your board `LOLIN(WEMOS) D1 R2 & mini` from Tools > Board menu

## Compile Project and upload

1. Open the `.ino` file in the Arduino IDE (your should have 6 tabs in the project)
2. Plug the **Wemos** to you computer
3. Select the port from Tools > Port
4. On the top-left side, click on the arrow to upload program
5. When uploaded, you can open the serial monitor Tools > Serial Monitor

Right now, the program should fail as the **Wemos** is not able to connect to you Wifi network

## Run the program

The program run since the **Wemos** is powered (via USB). For the first run process as following:

1. On your internet modem, push **WPS** button (sometimes, you have to configure it through the admin webpage)
2. Then quickly, power your **Wemos** via USB
3. Select the port from Tools > Port (it may have changed)
4. Open the serial monitor Tools > Serial Monitor

The WPS should succeed: should display a local IP address

```text
Try to connect
 
WPS config start
Trying to connect to <your modem> with saved config ...|SUCCESS
IP address: 192.168.xx.xx
```

Then the **Wemos** tries to connect to OVH server and perform a POST

```text
------------------------------------------------
POST opentsdb.gra1.metrics.ovh.net/api/put
[{"metric": "universe","value":42,"tags":{}}]
------------------------------------------------
beginResult: 0
http: 204
response: xxxx
```

if `beginResult` is negative, connection to OVH server failed. It may mean that the `FINGERPRINT` is wrong

if `http` is not `2xx` (should be `204`), the server could not treat your request. It may mean that the `TOKEN` is wrong

## Configure Grafana

Go to OVH Grafana: <https://grafana.metrics.ovh.net/login> and login with your OVH account

![step08](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/step08.png)

### Configure a data source

Click on "Add data source"

![Data source](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/grafana00.png)

Then fill the form:

* Name: choose one
* Type: OpenTSDB
* URL: https://{host you got from OVH manager}
* Access: direct
* Check "Basic Auth"
* User: metrics
* Password: <**read** token from OVH manager>

Click on Add

![Data source](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/grafana01.png)

All is OK ? Save it

![Data source](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/grafana02.png)

### Create your first chart

Go back to <https://grafana.metrics.ovh.net/> and click on "New Dashboard"

![Data source](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/grafana03.png)

Click on "Graph"

![Data source](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/grafana04.png)

Click on the title "Panel Title" and "Edit"

![Data source](https://github.com/landru29/ovh_metrics_wemos/raw/master/tutorial/img/grafana05.png)

Select your Metric in the field "metric name"; the software must propose you the name "universe". If not, this mean the metrics was not correctly sent by the wemos.

Close the edit panel (cross on the right) ans save your configuration (top left hand of the window)
