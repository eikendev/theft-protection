#include "authorizer.hpp"
#include "settings.hpp"

static const uint8_t tag01[] = {TAG01_UID};

Authorizer::Authorizer(void)
{
    this->mfrc522 = MFRC522(SS_PIN, RST_PIN);
}

void Authorizer::begin(void)
{
    this->mfrc522.PCD_Init();
    delay(4);
}

// Check if an authorized NFC tag is present.
bool Authorizer::is_authorized_nearby(void)
{
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        const byte *buffer = mfrc522.uid.uidByte;
        const size_t size = mfrc522.uid.size;

        if (size == sizeof(tag01) && memcmp(buffer, tag01, size) == 0) {
            return true;
        }
    }

    return false;
}
