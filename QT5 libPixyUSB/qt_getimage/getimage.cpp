//
//https://github.com/charmedlabs/pixy/blob/master/src/host/libpixyusb/src/pixy.cpp
//
//
#include <iostream>
#include <string>
#include <unistd.h>

#include "libpixyusb2.h"

using namespace std;

#define PIXY_ERROR_USB_IO                    LIBUSB_ERROR_IO
#define PIXY_ERROR_USB_NOT_FOUND             LIBUSB_ERROR_NOT_FOUND
#define PIXY_ERROR_USB_BUSY                  LIBUSB_ERROR_BUSY
#define PIXY_ERROR_USB_NO_DEVICE             LIBUSB_ERROR_NO_DEVICE
#define PIXY_ERROR_INVALID_PARAMETER        -150
#define PIXY_ERROR_CHIRP                    -151
#define PIXY_ERROR_INVALID_COMMAND          -152
static struct
{ int           error;
  const char *  text;
}
PIXY_ERROR_TABLE[] = {
  { 0,                          "Success" },
  { PIXY_ERROR_USB_IO,          "USB Error: I/O" },
  { PIXY_ERROR_USB_BUSY,        "USB Error: Busy" },
  { PIXY_ERROR_USB_NO_DEVICE,   "USB Error: No device" },
  { PIXY_ERROR_USB_NOT_FOUND,   "USB Error: Target not found" },
  { PIXY_ERROR_CHIRP,           "Chirp Protocol Error" },
  { PIXY_ERROR_INVALID_COMMAND, "Pixy Error: Invalid command" },
  { 0,                          0 }
};

Pixy2 pixy;
int iReturn, Result;


void pixy_error(const char *name, int error_code)
{
  int index;

    // Convert pixy error code to string and display to stdout //
    index = 0;
    while (PIXY_ERROR_TABLE[index].text != 0) {
        if (PIXY_ERROR_TABLE[index].error == error_code) {
            printf("%s: %s\n", name, PIXY_ERROR_TABLE[index].text);
            return;
            }
        index += 1;
        }
    printf("Undefined error: [%d]\n", error_code);
}

void getImage()
{
int return_value;
int32_t response;

    //Mat image;
    char fourcc_code[5];

    // Runs the Pixy to make it put a frame inside its video buffer
    //return_value = pixy.m_link.callChirp("runprog", 0x01, 8, END, &response, END);
    return_value = pixy.m_link.callChirp("run", END_OUT_ARGS, &response, END_IN_ARGS);
    pixy_error("run",return_value);
    if(return_value != 0){
        cout << "Can't run Pixy. Returned value : " << return_value << endl;
        //return image;
        return;
        }

    //sleep(1);
    // Stops the Pixy before asking it to send the frame stored inside its video buffer
    return_value = pixy.m_link.callChirp("stop", END_OUT_ARGS, &response, END_IN_ARGS);
    pixy_error("stop",return_value);
    if(return_value != 0){
        cout << "Can not stop Pixy. Returned value : " << return_value << endl;
        return;
        }

    /*
    cam_getFrame(INT8 mode, INT16 xOffset, INT16 yOffset, INT16 width, INT16 height)
    Get a frame from the camera
    Parameters:
       mode: one of the following CAM_GRAB_M0R0 (0x00), CAM_GRAB_M1R1 (0x11), CAM_GRAB_M1R2 (0x21)
       xOffset: x offset counting from left
       yOffset: y offset counting from top
       width: width of frame
       height: height of frame
    Returns:
       0 if success, negative if error
       BA81 formatted data */
    //int Renderer::renderBA81(uint8_t renderFlags, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame)
         uint8_t* videodata;
         //int32_t response;
         int32_t fourcc;
         int8_t renderflags;
         uint16_t xwidth;
         uint16_t ywidth;
         uint32_t size;

         return_value = pixy.m_link.callChirp("cam_getFrame",  // String id for remote procedure
                                         0x01,0x21,     // mode
                                         0x02,0,        // xoffset
                                         0x02,0,         // yoffset
                                         0x02,316,       // width
                                         0x02,208,       // height
                                         END_OUT_ARGS,              // separator
                                         &response,      // pointer to mem address for return value
                                         &fourcc,
                                         &renderflags,
                                         &xwidth,
                                         &ywidth,
                                         &size,
                                         &videodata,        // pointer to mem address for returned frame
                                         END_IN_ARGS);
         pixy_error("getFrame",return_value);
    // Check that the Pixy successfully returned a frame
    if(return_value != 0) {
        cout << "Can not get frame from Pixy. Returned value : " << return_value << endl;
        return ;
        }
    // Convert fourcc code into string
    fourcc_code[0] = fourcc & 0xff;
    fourcc_code[1] = (fourcc >> 8) & 0xff;
    fourcc_code[2] = (fourcc >> 16) & 0xff;
    fourcc_code[3] = (fourcc >> 24) & 0xff;
    fourcc_code[4] = '\0';
    // Display the different values returned by the Pixy
    //cout << "Cam_GetFrame Pixy: " << return_value << endl;
    cout << "Response returned by Pixy : " << response << ", fourcc : " << fourcc_code << ", renderflags : " << renderflags << ", width : " << xwidth << ", height : " << ywidth << ", numPixels : " << size << endl;
    // Render frame sent by Pixy
    //image = renderBA81(renderflags,rwidth,rheight,numPixels,pixels);
    //return image;
    return;
}


int main(int argc, char * argv[])
{

    // Initilize Pixy--------------------------
    iReturn = pixy.init();
    pixy_error("Pixy init",iReturn);
    if (iReturn<0) return iReturn;

    // Get F/W H/W Version---------------------
    uint16_t  *pixy_version;
    uint16_t  v[6];
    uint32_t  version_length;
    uint32_t  response;
    iReturn = pixy.m_link.callChirp("version", END_OUT_ARGS, &response, &version_length, &pixy_version,  END_IN_ARGS);
    if (iReturn >= 0) {
        memcpy((void *)v,  pixy_version, 6 * sizeof(uint16_t));
        printf("Version F/W:%u.%u.%u  H/W:%u.%u.%u\n", v[0],v[1],v[2], v[3],v[4],v[5]);
        }
    //-----------------------------------------
    iReturn = pixy.m_link.callChirp("led_setLamp", INT8(1), INT8(0), END_OUT_ARGS, &Result, END_IN_ARGS);
    pixy_error("led_setLamp Upper",iReturn);
    sleep(1);

    iReturn = pixy.m_link.callChirp("led_setLamp", INT8(0), INT8(1), END_OUT_ARGS, &Result, END_IN_ARGS);
    pixy_error("led_setLamp Lower",iReturn);
/*
    // Stop the Pixy
    iReturn = pixy.m_link.callChirp("stop", END_OUT_ARGS, &Result, END_IN_ARGS);
    pixy_error("stop",iReturn);

    // Exercise LED-------------------------------
    // Pack the CMY value //
    //Cyan      0xFF0000
    //Magenta   0x00FF00
    //Yellow    0x0000FF
    //Red       0x00FFFF
    //Green     0xFF00FF
    //Blue      0xFFFF00
    iReturn = pixy.m_link.callChirp("led_setRGB", INT8(0), INT8(0), INT8(0), END_OUT_ARGS, &Result, END_IN_ARGS);
    iReturn = pixy.m_link.callChirp("led_setLamp", INT8(0), INT8(1), END_OUT_ARGS, &Result, END_IN_ARGS);
    iReturn = pixy.m_link.callChirp("led_setRGB", INT8(255), INT8(0), INT8(0), END_OUT_ARGS, &Result, END_IN_ARGS);
    sleep(1);
    iReturn = pixy.m_link.callChirp("led_setRGB", INT8(0), INT8(0), INT8(0), END_OUT_ARGS, &Result, END_IN_ARGS);
    iReturn = pixy.m_link.callChirp("led_setLamp", INT8(0), INT8(1), END_OUT_ARGS, &Result, END_IN_ARGS);
    iReturn = pixy.m_link.callChirp("led_setRGB", INT8(0), INT8(255), INT8(0), END_OUT_ARGS, &Result, END_IN_ARGS);
    sleep(1);
    iReturn = pixy.m_link.callChirp("led_setRGB", INT8(0), INT8(0), INT8(0), END_OUT_ARGS, &Result, END_IN_ARGS);;
    iReturn = pixy.m_link.callChirp("led_setLamp", INT8(0), INT8(1), END_OUT_ARGS, &Result, END_IN_ARGS);
    iReturn = pixy.m_link.callChirp("led_setRGB", INT8(0), INT8(0), INT8(255), END_OUT_ARGS, &Result, END_IN_ARGS);
    sleep(1);

    iReturn = pixy.m_link.callChirp("led_setRGB", INT8(0), INT8(0), INT8(0), END_OUT_ARGS, &Result, END_IN_ARGS);
    iReturn = pixy.m_link.callChirp("led_setLamp", INT8(0), INT8(1), END_OUT_ARGS, &Result, END_IN_ARGS);
    iReturn = pixy.m_link.callChirp("led_setRGB", INT8(0), INT8(255), INT8(255), END_OUT_ARGS, &Result, END_IN_ARGS);
    sleep(1);
    iReturn = pixy.m_link.callChirp("led_setRGB", INT8(0), INT8(0), INT8(0), END_OUT_ARGS, &Result, END_IN_ARGS);
    iReturn = pixy.m_link.callChirp("led_setLamp", INT8(0), INT8(1), END_OUT_ARGS, &Result, END_IN_ARGS);
    iReturn = pixy.m_link.callChirp("led_setRGB", INT8(255), INT8(0), INT8(255), END_OUT_ARGS, &Result, END_IN_ARGS);
    sleep(1);
    iReturn = pixy.m_link.callChirp("led_setRGB", INT8(0), INT8(0), INT8(0), END_OUT_ARGS, &Result, END_IN_ARGS);
    iReturn = pixy.m_link.callChirp("led_setLamp", INT8(0), INT8(1), END_OUT_ARGS, &Result, END_IN_ARGS);
    iReturn = pixy.m_link.callChirp("led_setRGB", INT8(255), INT8(255), INT8(0), END_OUT_ARGS, &Result, END_IN_ARGS);
    sleep(1);
    //-----------------------------------------
*/

    getImage();

    return 0;
}

/*
int main1(int argc, char** argv) {

    uint8_t Purple_Color = 0xFF;
    uint8_t Green_Color = 0x00;
    uint32_t Color;
    Color = (Purple_Color << 16) + (Green_Color << 8) + Purple_Color;

    estadopixy = camarapixy.init();
    if (estadopixy) {
        cout << "Error de conexion con Pixy, seguro que esta conectada?" << endl;
        return -1;
    }

    //cout << "Version: " << camarapixy->version << std::endl;


    int return_value = camarapixy.m_link.callChirp("led_set", INT32(Color), END_OUT_ARGS, &Result, END_IN_ARGS);
    errores(return_value, "Led_set");

    // Start last running program on Pixy //
    //return_value = camarapixy.m_link.callChirp("run", END_OUT_ARGS, &Result, END_IN_ARGS);
    //errores(return_value, "run");
    // Stop running a program on Pixy //
    return_value = camarapixy.m_link.callChirp("stop", END_OUT_ARGS, &Result, END_IN_ARGS);
    errores(return_value, "stop");

    int32_t response;
    uint32_t fourcc;
    int8_t renderflags;
    uint16_t width, height;
    uint32_t numPixels;
    unsigned char *frame;


    return_value = camarapixy.m_link.callChirp("cam_getFrame", // String id for remote procedure
            INT8(0x21), // mode
            INT16(0), // xoffset
            INT16(0), // yoffset
            INT16(300), // width
            INT16(200), // height
            END_OUT_ARGS,
            &response, // pointer to mem address for return value
            &fourcc,
            &renderflags,
            &width,
            &height,
            &numPixels,
            &frame, // pointer to mem address for returned frame
            END_IN_ARGS
            );

    errores(return_value, "cam_getFrame");
    cout << "Respuesta camara: " << response << endl;
    cout << "Codec FourCC: " << fourcc << endl;
    cout << "Render flags: " << renderflags << endl;
    cout << "width: " << width << endl;
    cout << "height: " << height << endl;
    cout << "Numero Pixeles: " << numPixels << endl;
    cout << "Captura: " << frame << endl;


    return_value = camarapixy.m_link.callChirp("version");
    errores(return_value, "Version");
    return_value = camarapixy.m_link.callChirp("led_setRGB", INT8(55), INT8(125), INT8(255));
    errores(return_value, "led_setRGB");
    sleep(3);
    return_value = camarapixy.m_link.callChirp("led_setRGB", INT8(255), INT8(125), INT8(5));
    errores(return_value, "led_set_RGB");
    sleep(3);
    return 0;
}

*/
