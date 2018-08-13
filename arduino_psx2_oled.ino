#include <PS2X_lib.h> 
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>


PS2X ps2x;

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

#define PSX2_DATA 12      // D0 or PIN 1 on PSX controller
#define PSX2_CLOCK 11     // D1 or PIN 2 on PSX controller
#define PSX2_ATTENTION 10 // CS or PIN 6 on PSX controller
#define PSX2_COMMAND 9    // CLK or PIN 7 on PSX controller

#define PSX2_PRESSURES true
#define PSX2_RUMBLE false

#define OLED_I2C_ADDRESS 0x3C
SSD1306AsciiWire display;

void setupDisplay()
{
    display.begin(&Adafruit128x64, OLED_I2C_ADDRESS);
    display.set400kHz();
    display.setFont(Adafruit5x7);
    display.clear();
    display.setCursor(0, 0);
}

void setup()
{
    Serial.begin(57600);
    setupDisplay();

    error = ps2x.config_gamepad(
        PSX2_CLOCK,
        PSX2_COMMAND,
        PSX2_ATTENTION,
        PSX2_DATA,
        PSX2_PRESSURES,
        PSX2_RUMBLE);

    // check for error
    if (error == 0) {
        Serial.println("Found Controller, configured successful");
        Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
        Serial.println("holding L1 or R1 will print out the analog stick values.");
        Serial.println("Go to www.billporter.info for updates and to report bugs.");
    }

    else if (error == 1)
        Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

    else if (error == 2)
        Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

    else if (error == 3)
        Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

    //Serial.print(ps2x.Analog(1), HEX);

    type = ps2x.readType();
    switch (type) {
    case 0:
        Serial.println("Unknown Controller type");
        break;
    case 1:
        Serial.println("DualShock Controller Found");
        display.println(F("DualShock"));
        display.println();
        break;
    case 2:
        Serial.println("GuitarHero Controller Found");
        break;
    }

}

void displayButton(char* msg)
{
    display.setCursor(0, 1);
    display.clearToEOL();
    display.print(msg);
}

void displayStick(uint8_t row, byte x, byte y)
{
    display.setCursor(0, row);
    display.clearToEOL();
    display.print(x, DEC);
    display.print(", ");
    display.print(y, DEC);
}

byte prevLX = 0;
byte prevLY = 0;

byte prevRX = 0;
byte prevRY = 0;


void displayLeftStick(byte x, byte y)
{
    //if (prevLX != x || prevLY != y)
    {
        prevLX = x;
        prevLY = y;
        displayStick(2, x, y);
    }
}

void displayRightStick(byte x, byte y)
{
    //if (prevRX != x || prevRY != y)
    {
        prevRX = x;
        prevRY = y;
        displayStick(3, x, y);
    }
}

void displayDPad(char* msg)
{
    display.setCursor(0, 4);
    display.clearToEOL();
    display.print("DPad: ");
    display.print(msg);
}

void loop() {

    if (error == 1)
    {
        return;
    }

    if (type == 2) //Guitar Hero Controller
    {

        ps2x.read_gamepad();

        if (ps2x.ButtonPressed(GREEN_FRET))
            Serial.println("Green Fret Pressed");
        if (ps2x.ButtonPressed(RED_FRET))
            Serial.println("Red Fret Pressed");
        if (ps2x.ButtonPressed(YELLOW_FRET))
            Serial.println("Yellow Fret Pressed");
        if (ps2x.ButtonPressed(BLUE_FRET))
            Serial.println("Blue Fret Pressed");
        if (ps2x.ButtonPressed(ORANGE_FRET))
            Serial.println("Orange Fret Pressed");


        if (ps2x.ButtonPressed(STAR_POWER))
            Serial.println("Star Power Command");

        if (ps2x.Button(UP_STRUM))          //will be TRUE as long as button is pressed
            Serial.println("Up Strum");
        if (ps2x.Button(DOWN_STRUM))
            Serial.println("DOWN Strum");


        if (ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
            Serial.println("Start is being held");
        if (ps2x.Button(PSB_SELECT))
            Serial.println("Select is being held");


        if (ps2x.Button(ORANGE_FRET)) // print stick value IF TRUE
        {
            Serial.print("Wammy Bar Position:");
            Serial.println(ps2x.Analog(WHAMMY_BAR), DEC);
        }
    }
    else // DualShock Controller 
    {
        ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

        if (ps2x.Button(PSB_START))
        {
            //will be TRUE as long as button is pressed
            displayButton("Start");
        }

        if (ps2x.Button(PSB_SELECT))
        {
            displayButton("Select");
        }


        if (ps2x.Button(PSB_PAD_UP)) 
        {
            //will be TRUE as long as button is pressed
            displayDPad("Up");
        }
        if (ps2x.Button(PSB_PAD_RIGHT)) 
        {
            displayDPad("Right");
        }
        if (ps2x.Button(PSB_PAD_LEFT)) 
        {
            displayDPad("Left");

        }
        if (ps2x.Button(PSB_PAD_DOWN)) 
        {
            displayDPad("Down");
        }

        //will be TRUE if any button changes state (on to off, or off to on)
        if (ps2x.NewButtonState())               
        {
            if (ps2x.Button(PSB_L1))
            {
                displayButton("L1");
            }
            if (ps2x.Button(PSB_R1))
            {
                displayButton("R1");
            }
            if (ps2x.Button(PSB_L2))
            {
                displayButton("L2");
            }
            if (ps2x.Button(PSB_R2))
            {
                displayButton("R2");
            }
            if (ps2x.Button(PSB_L3))
            {
                displayButton("L3");
            }
            if (ps2x.Button(PSB_R3))
            {
                displayButton("R3");
            }
            if (ps2x.Button(PSB_GREEN))
            {
                displayButton("Triangle");
            }
            if (ps2x.Button(PSB_RED))
            {
                displayButton("Circle");
            }
            if (ps2x.Button(PSB_PINK))
            {
                displayButton("Square");
            }
            if (ps2x.Button(PSB_BLUE))
            {
                displayButton("X");
            }
        }

        displayLeftStick(ps2x.Analog(PSS_LX), ps2x.Analog(PSS_LY));
        displayRightStick(ps2x.Analog(PSS_RX), ps2x.Analog(PSS_RY));

        delay(1);
    }

}
