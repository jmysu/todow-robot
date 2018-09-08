# QT5 libPixyUSB and examples
---

PIXY2 libPixyUSB stuff w/ Qt5

for share library output path, add "DESTDIR = lib" into .pro


**for MAC**: remember add libPixyUSB build path into Qt project _DYLD_LIBRARY_PATH_ when running at OSX

> 
## Dependencies
- [QT5](https://github.com/qt/qt5)
- [PIXY2](https://github.com/charmedlabs/pixy2)
- [libUSB](https://github.com/libusb/libusb)

## Log
- qt_chirp OK!
- qt_getimage failed on getting cam_getFrame response, return -3!
