
#include "app/setting/bluetooth.h"
bool bluetooth_s=0;
String BLUETOOTH_name="ESP_BLE";
bool BLE_adverting=true;
uint8_t client_count=0;
#if 0
class BLEserver_cb: public BLEServerCallbacks
{
    void onConnect()
    {
        
        client_count++;
        BLE_adverting=false;
    }
    void onDisconnect()
    {
        client_count--;
    }
};

void bluetooth_check(void *p)
{
    while(bluetooth_s)
    {
        if(!BLE_adverting&&client_count<1&&BLEDevice::getInitialized())
        {
            delay(500);
            BLEDevice::startAdvertising();
            BLE_adverting=true;
        }
        delay(100);
    }
    vTaskDelete(NULL);
}
BLEServer* pserver;
BLEService *pservice;
void bluetooth_init()
{
    BLEDevice::init("12334");
    auto local_adress=BLEDevice::getAddress();

    pserver=BLEDevice::createServer();
    pserver->setCallbacks(new BLEserver_cb());
    pservice=pserver->createService(SERVICE_UUID);
    BLECharacteristic *characteristic=pservice->createCharacteristic(CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE);

    characteristic->setValue("LCC_ESP_BLE");
    pservice->start();

    BLEAdvertising* advertising=BLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->setScanResponse(true);
    advertising->setMinPreferred(0x12);

    BLEDevice::startAdvertising();
    
    xTaskCreatePinnedToCore(bluetooth_check,NULL,4096,NULL,10,NULL,0);
}

void bluetooth_close()
{
    if(BLEDevice::getInitialized())
    {
        if(BLE_adverting) BLEDevice::stopAdvertising();
        uint16_t nnid=pserver->getConnId();
        pservice->stop();
        pserver->disconnect(nnid);
        pserver->removeService(pservice);
        BLEDevice::deinit(true);
    }
    BLE_adverting=1;
    client_count=0;
}

lv_obj_t*setting_bluetooth_load()
{
    now_screen =lv_obj_create(NULL);
    lv_obj_set_style_bg_color(now_screen,lv_color_hex(0x000000), LV_STATE_DEFAULT);
    lv_obj_set_size(now_screen, screenWidth, screenHeight); 

    return now_screen;
}

#endif