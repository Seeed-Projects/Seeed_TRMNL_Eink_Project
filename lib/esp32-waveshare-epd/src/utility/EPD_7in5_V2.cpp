/*****************************************************************************
* | File      	:	EPD_7in5.c
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* |	This version:   V2.0
* | Date        :   2018-11-09
* | Info        :
*****************************************************************************
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documnetation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to  whom the Software is
// furished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
******************************************************************************/
#include "EPD_7in5_V2.h"
#include "Debug.h"

UBYTE Voltage_Frame_7IN5_V2[] = {
    0x6,
    0x3F,
    0x3F,
    0x11,
    0x24,
    0x7,
    0x17,
};

UBYTE LUT_VCOM_7IN5_V2[] = {
    0x0,
    0xF,
    0xF,
    0x0,
    0x0,
    0x1,
    0x0,
    0xF,
    0x1,
    0xF,
    0x1,
    0x2,
    0x0,
    0xF,
    0xF,
    0x0,
    0x0,
    0x1,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
};

UBYTE LUT_WW_7IN5_V2[] = {
    0x10,
    0xF,
    0xF,
    0x0,
    0x0,
    0x1,
    0x84,
    0xF,
    0x1,
    0xF,
    0x1,
    0x2,
    0x20,
    0xF,
    0xF,
    0x0,
    0x0,
    0x1,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
};

UBYTE LUT_BW_7IN5_V2[] = {
    0x10,
    0xF,
    0xF,
    0x0,
    0x0,
    0x1,
    0x84,
    0xF,
    0x1,
    0xF,
    0x1,
    0x2,
    0x20,
    0xF,
    0xF,
    0x0,
    0x0,
    0x1,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
};

UBYTE LUT_WB_7IN5_V2[] = {
    0x80,
    0xF,
    0xF,
    0x0,
    0x0,
    0x3,
    0x84,
    0xF,
    0x1,
    0xF,
    0x1,
    0x4,
    0x40,
    0xF,
    0xF,
    0x0,
    0x0,
    0x3,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
};

UBYTE LUT_BB_7IN5_V2[] = {
    0x80,
    0xF,
    0xF,
    0x0,
    0x0,
    0x1,
    0x84,
    0xF,
    0x1,
    0xF,
    0x1,
    0x2,
    0x40,
    0xF,
    0xF,
    0x0,
    0x0,
    0x1,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
};

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_Reset(void)
{
#ifdef BOARD_XIAO_EPAPER_DISPLAY
    DEV_Digital_Write(EPD_RST_PIN, 1);
#else
    REG_WRITE(GPIO_OUT_W1TS_REG, 1 << EPD_RST_PIN);
#endif

    DEV_Delay_ms(200);
    
#ifdef BOARD_XIAO_EPAPER_DISPLAY
    DEV_Digital_Write(EPD_RST_PIN, 0);
#else
    REG_WRITE(GPIO_OUT_W1TC_REG, 1 << EPD_RST_PIN);
#endif

    DEV_Delay_ms(2);

#ifdef BOARD_XIAO_EPAPER_DISPLAY
    DEV_Digital_Write(EPD_RST_PIN, 1);
#else
    REG_WRITE(GPIO_OUT_W1TS_REG, 1 << EPD_RST_PIN);
#endif

    DEV_Delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_SendCommand(UBYTE Reg)
{
#ifdef BOARD_XIAO_EPAPER_DISPLAY
    DEV_Digital_Write(EPD_DC_PIN, 0);
#else
    REG_WRITE(GPIO_OUT_W1TC_REG, 1 << EPD_DC_PIN);
#endif

    DEV_SPI_WriteByte(Reg);

    //DEV_Digital_Write(EPD_CS_PIN, 1);
    //REG_WRITE(GPIO_OUT_W1TS_REG, 1 << EPD_CS_PIN);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_SendData(UBYTE Data)
{
#ifdef BOARD_XIAO_EPAPER_DISPLAY
    DEV_Digital_Write(EPD_DC_PIN, 1);
#else
    REG_WRITE(GPIO_OUT_W1TS_REG, 1 << EPD_DC_PIN);
#endif

    DEV_SPI_WriteByte(Data);
}

static void EPD_SendData2(uint16_t Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void EPD_WaitUntilIdle(void)
{
    Debug("e-Paper busy\r\n");
    unsigned char busy;

    while (1) {
        if (gpio_get_level( (gpio_num_t)EPD_BUSY_PIN ) == 1) break;
        delay(1);  //give CPU to WiFi stack
    }

    Debug("e-Paper busy release\r\n");
}

static void EPD_7IN5_V2_LUT(UBYTE *lut_vcom, UBYTE *lut_ww, UBYTE *lut_bw, UBYTE *lut_wb, UBYTE *lut_bb)
{
    UBYTE count;

    EPD_SendCommand(0x20); // VCOM
    for (count = 0; count < 60; count++)
        EPD_SendData(lut_vcom[count]);

    EPD_SendCommand(0x21); // LUTBW
    for (count = 0; count < 60; count++)
        EPD_SendData(lut_ww[count]);

    EPD_SendCommand(0x22); // LUTBW
    for (count = 0; count < 60; count++)
        EPD_SendData(lut_bw[count]);

    EPD_SendCommand(0x23); // LUTWB
    for (count = 0; count < 60; count++)
        EPD_SendData(lut_wb[count]);

    EPD_SendCommand(0x24); // LUTBB
    for (count = 0; count < 60; count++)
        EPD_SendData(lut_bb[count]);
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_7IN5_V2_TurnOnDisplay(void)
{
    EPD_SendCommand(0x12); // DISPLAY REFRESH
    delayMicroseconds(500);    //!!!The delay here is necessary, 200uS at least!!!
    //EPD_WaitUntilIdle();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
UBYTE EPD_7IN5_V2_Init(void)
{
    EPD_Reset();

    // EPD_SendCommand(0x01);			//POWER SETTING
    // EPD_SendData(0x07);
    // EPD_SendData(0x07);    //VGH=20V,VGL=-20V
    // EPD_SendData(0x3f);		//VDH=15V
    // EPD_SendData(0x3f);		//VDL=-15V

    EPD_SendCommand(0x01);                      // power setting
    EPD_SendData(0x17);                         // 1-0=11: internal power
    EPD_SendData(*(Voltage_Frame_7IN5_V2 + 6)); // VGH&VGL
    EPD_SendData(*(Voltage_Frame_7IN5_V2 + 1)); // VSH
    EPD_SendData(*(Voltage_Frame_7IN5_V2 + 2)); //  VSL
    EPD_SendData(*(Voltage_Frame_7IN5_V2 + 3)); //  VSHR

    EPD_SendCommand(0x82);                      // VCOM DC Setting
    EPD_SendData(*(Voltage_Frame_7IN5_V2 + 4)); // VCOM

    EPD_SendCommand(0x06); // Booster Setting
    EPD_SendData(0x27);
    EPD_SendData(0x27);
    EPD_SendData(0x2F);
    EPD_SendData(0x17);

    EPD_SendCommand(0x04); // POWER ON
    DEV_Delay_ms(100);
    EPD_WaitUntilIdle();

    EPD_SendCommand(0X00); // PANNEL SETTING
    EPD_SendData(0x3F);    // KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

    EPD_SendCommand(0x61); // tres
    EPD_SendData(0x03);    // source 800
    EPD_SendData(0x20);
    EPD_SendData(0x01); // gate 480
    EPD_SendData(0xE0);

    EPD_SendCommand(0X15);
    EPD_SendData(0x00);

    EPD_SendCommand(0X50); // VCOM AND DATA INTERVAL SETTING
    EPD_SendData(0x10);
    EPD_SendData(0x00);

    EPD_SendCommand(0X60); // TCON SETTING
    EPD_SendData(0x22);

    EPD_SendCommand(0x65); // Resolution setting
    EPD_SendData(0x00);
    EPD_SendData(0x00); // 800*480
    EPD_SendData(0x00);
    EPD_SendData(0x00);

    EPD_7IN5_V2_LUT(LUT_VCOM_7IN5_V2, LUT_WW_7IN5_V2, LUT_BW_7IN5_V2, LUT_WB_7IN5_V2, LUT_BB_7IN5_V2);

    return 0;
}

UBYTE EPD_7IN5_V2_Init_New(void)
{
    EPD_Reset();

    // EPD_SendCommand(0x01);			//POWER SETTING
    // EPD_SendData(0x07);
    // EPD_SendData(0x07);    //VGH=20V,VGL=-20V
    // EPD_SendData(0x3f);		//VDH=15V
    // EPD_SendData(0x3f);		//VDL=-15V

    EPD_SendCommand(0x06); // btst
    EPD_SendData(0x17);
    EPD_SendData(0x17);
    EPD_SendData(0x28); // If an exception is displayed, try using 0x38
    EPD_SendData(0x17);

    EPD_SendCommand(0x01); // POWER SETTING
    EPD_SendData(0x07);
    EPD_SendData(0x07); // VGH=20V,VGL=-20V
    EPD_SendData(0x3f); // VDH=15V
    EPD_SendData(0x3f); // VDL=-15V

    EPD_SendCommand(0x04); // POWER ON
    DEV_Delay_ms(100);
    EPD_WaitUntilIdle();

    EPD_SendCommand(0x00); // PANNEL SETTING
    EPD_SendData(0x1F);    // KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

    EPD_SendCommand(0x61); // tres
    EPD_SendData(0x03);    // source 800
    EPD_SendData(0x20);
    EPD_SendData(0x01); // gate 480
    EPD_SendData(0xE0);

    EPD_SendCommand(0x15);
    EPD_SendData(0x00);

    EPD_SendCommand(0x50); // VCOM AND DATA INTERVAL SETTING
    EPD_SendData(0x10);
    EPD_SendData(0x07);

    EPD_SendCommand(0x60); // TCON SETTING
    EPD_SendData(0x22);

    // EPD_7IN5_V2_LUT(LUT_VCOM_7IN5_V2, LUT_WW_7IN5_V2, LUT_BW_7IN5_V2, LUT_WB_7IN5_V2, LUT_BB_7IN5_V2);

    return 0;
}

UBYTE EPD_7IN5_V2_Init_Fast(void)
{
    // EPD hardware init start
    EPD_Reset();

    EPD_SendCommand(0x00); // PANNEL SETTING
    EPD_SendData(0x1F);    // KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

    EPD_SendCommand(0x50); // VCOM AND DATA INTERVAL SETTING
    EPD_SendData(0x10);
    EPD_SendData(0x07);

    EPD_SendCommand(0x04); // POWER ON
    DEV_Delay_ms(100);
    EPD_WaitUntilIdle(); // waiting for the electronic paper IC to release the idle signal

    // Enhanced display drive(Add 0x06 command)
    EPD_SendCommand(0x06); // Booster Soft Start
    EPD_SendData(0x27);
    EPD_SendData(0x27);
    EPD_SendData(0x18);
    EPD_SendData(0x17);

    EPD_SendCommand(0xE0);
    EPD_SendData(0x02);
    EPD_SendCommand(0xE5);
    EPD_SendData(0x5A);

    // EPD hardware init end
    return 0;
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_7IN5_V2_Clear(void)
{
    EPD_WaitUntilIdle();
    /*
    UWORD Width, Height;
    Width = (EPD_7IN5_V2_WIDTH % 8 == 0) ? (EPD_7IN5_V2_WIDTH / 8) : (EPD_7IN5_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5_V2_HEIGHT;

    UWORD i;
    EPD_SendCommand(0x10);
    for (i = 0; i < Height * Width; i++)
    {
        EPD_SendData(0xff);
    }
    EPD_SendCommand(0x13);
    for (i = 0; i < Height * Width; i++)
    {
        EPD_SendData(0x00);
    }
    */
    EPD_7IN5_V2_TurnOnDisplay();
}

void EPD_7IN5_V2_ClearBlack(void)
{
    EPD_WaitUntilIdle();

    UWORD Width, Height;
    Width = (EPD_7IN5_V2_WIDTH % 8 == 0) ? (EPD_7IN5_V2_WIDTH / 8) : (EPD_7IN5_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5_V2_HEIGHT;

    UWORD i;
    // EPD_SendCommand(0x10);
    // for (i = 0; i < Height * Width; i++)
    // {
    //     EPD_SendData(0x00);
    // }
    EPD_SendCommand(0x13);
    for (i = 0; i < Height * Width; i++)
    {
        EPD_SendData(0xFF);
    }
    EPD_7IN5_V2_TurnOnDisplay();
}

void EPD_7IN5_V2_ClearWhite(void)
{
    EPD_WaitUntilIdle();

    UWORD Width, Height;
    Width = (EPD_7IN5_V2_WIDTH % 8 == 0) ? (EPD_7IN5_V2_WIDTH / 8) : (EPD_7IN5_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5_V2_HEIGHT;

    UWORD i;
    // EPD_SendCommand(0x10);
    // for (i = 0; i < Height * Width; i++)
    // {
    //     EPD_SendData(0x00);
    // }
    EPD_SendCommand(0x13);
    for (i = 0; i < Height * Width; i++)
    {
        EPD_SendData(0x00);
    }
    EPD_7IN5_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_7IN5_V2_Display(const UBYTE *blackimage)
{
    EPD_WaitUntilIdle();
    UDOUBLE Width, Height;
    Width = (EPD_7IN5_V2_WIDTH % 8 == 0) ? (EPD_7IN5_V2_WIDTH / 8) : (EPD_7IN5_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5_V2_HEIGHT;

    // EPD_SendCommand(0x10);
    // for (UDOUBLE j = 0; j < Height; j++)
    // {
    //     for (UDOUBLE i = 0; i < Width; i++)
    //     {
    //         EPD_SendData(~blackimage[i + j * Width]);
    //     }
    // }
    
    // send black data
    EPD_SendCommand(0x13);
    for (UDOUBLE j = 0; j < Height; j++)
    {
        for (UDOUBLE i = 0; i < Width; i++)
        {
            EPD_SendData(~blackimage[i + j * Width]);
        }
    }
    EPD_7IN5_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_7IN5_V2_Sleep(void)
{
    EPD_WaitUntilIdle();
    EPD_SendCommand(0X02); // power off
    EPD_WaitUntilIdle();
    EPD_SendCommand(0X07); // deep sleep
    EPD_SendData(0xA5);
}
