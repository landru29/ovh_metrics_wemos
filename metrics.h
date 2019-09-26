#ifndef __METRICS__H__
#define __METRICS__H__

class Metrics {
  public:
    Metrics();
    void sendData(const char* name, double value);

};

#endif
