#ifndef __AUTHORIZER_HPP__
#define __AUTHORIZER_HPP__

#include <Arduino.h>
#include <MFRC522.h>

class Authorizer {
  private:
    MFRC522 mfrc522;

  public:
    Authorizer(void);

    void begin(void);

    bool is_authorized_nearby(void);
};

#endif
