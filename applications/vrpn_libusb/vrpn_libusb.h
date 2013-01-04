#ifndef VRPN_LIBUSB_H

#include "vrpn_Button.h"
#include "vrpn_Analog.h"

#include <libusb-1.0/libusb.h>
#include <pthread.h>

#include <string>

struct ThreadInit;

class VRPN_API vrpn_libusb : public vrpn_Button, public vrpn_Analog
{
    public:
        vrpn_libusb(const char *name, vrpn_Connection *c, std::string configFile);
        virtual ~vrpn_libusb();

        bool isError();

        virtual void mainloop();

        int getNumButtons() { return num_buttons; }

        pthread_mutex_t _updateLock;
        int _buttonOffset;
        int _wheelOffset;
        bool _withWheel;
        bool _buttonOffsetSet;
        bool _withXY;
        int _xOffset;
        int _yOffset;
        int _packetSize;
        float * _valLocal;
        int * _buttonLocal;

        int _wheelIndex;
        int _xIndex;
        int _yIndex;

        struct timeval _lastUpdateTime;
    protected:
        bool loadConfigFile(std::string & configFile);

        void printDevices(libusb_device ** list);
        libusb_device * findDevice(libusb_device ** list);
        void printConfig();

        libusb_context * _context;

        libusb_device ** _devList;
        libusb_device * _dev;
        libusb_device_handle * _handle;
        int _vendorID;
        int _productID;
        int _entryNum;
        int _configNum;
        int _numConfigs;
        int _interfaceNum;
        int _numInterfaces;
        int _altSettingNum;
        int _numAltSettings;
        int _endpointNum;
        int _numEndpoints;
        int _address;
        float _wheelTimeout;
        float _xyTimeout;

        bool _error;

        pthread_t _updateThread;
        struct ThreadInit * _ti;
        unsigned char * _packet;
        bool _driverPresent;

        libusb_transfer * _transfer;
};

void transCallback(struct libusb_transfer * transfer);

void * libusbUpdate(void * init);

struct ThreadInit
{
    libusb_context * context;
    bool quit;
    pthread_mutex_t _quitLock;
};

#endif
