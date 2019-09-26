#ifndef __WIFI_CONNECT__H__
#define __WIFI_CONNECT__H__

class WifiConnect {
  public:
    WifiConnect(const char* hostname);
    bool startWPSPBC();

  private:
    void pauseConnect();
    void connectSuccess();
    void connectFailed();
    void stepFoolConnect();
    void startWps();
    unsigned char ledState;
};

#endif
