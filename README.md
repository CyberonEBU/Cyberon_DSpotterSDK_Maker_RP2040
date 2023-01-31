# Cyberon_DSpotterSDK_Maker

## Descriptions

* Cyberon DSpotterSDK Maker, providing local voice trigger and command recognition function on Arduino supported boards for Arduino developers.

* Below are the features of Cyberon DSpotterSDK Maker:
  * Phoneme-based speaker-independent command recognition.
  * DNN-based model that provides high accuracy and noise-robustness.
  * No need to collect speech data for training. Custom command set can be easily created through text input.
  * Offline keyword spotting with low power consumption.
  * More than 26 languages supported.

## Supported Arduino Boards

* [Arduino Nano RP2040 Connect (ARM Cortex-M0+)](https://store.arduino.cc/products/arduino-nano-rp2040-connect)
* [Arduino Nano 33 BLE Sense (ARM Cortex-M4)](https://store.arduino.cc/products/arduino-nano-33-ble-sense) 
* [Arduino Portenta H7 (ARM Cortex-M7)](https://store.arduino.cc/products/portenta-h7)

## Quick Start

### Installation

#### Boards Installation

1. Go to `Tools -> Board` in the Arduino IDE.
2. Install `Arduino Mbed OS Nano Boards` for `Arduino Nano RP2040 Connect` & `Arduino Nano 33 BLE Sense` board.
3. Install `Arduino Mbed OS Portenta Boards` for `Arduino Portenta H7` board.
4. Connect the board to the computer.
5. Select the board type in `Tools -> Board`
6. Select the serial port that connect to the board in `Tools -> Port`.

#### Library Installation

There are three ways to install Cyberon_DSpotterSDK_Maker library:

* Install by Arduino Library Manager:
    1. Go to `Tool -> Manage Libraries...` and type `Cyberon_DSpotterSDK_Maker` in the search bar.
    2. Click install, the `Cyberon_DSpotterSDK_Maker` will be installed automatically to the `library` folder under the `sketchbook location` path.
* Add the .ZIP library:
    1. Go to  `Sketch -> Include Library -> Add .ZIP Library...`.
    2. Select the .ZIP library file `Cyberon_DSpotterSDK_Maker.zip` and Click `Open`.
    3. The `Cyberon_DSpotterSDK_Maker` will be extracted to the `library` folder under the `sketchbook location` path.
* Manually add the library:
    1. Extract the `Cyberon_DSpotterSDK_Maker.zip` file to the `library` folder under the `sketchbook location` path.

### Get the DeviceID of the Board

* `Verify & Upload` the `GetSerialNumber.ino` example sketch to get the board's serial number and print it on the console output.

### Get the Free Demo License of the DeviceID

1. Go to the [GetLicense Page](https://tool.cyberon.com.tw/GetLicense/GetLicensePage.php).

2. Enter the board's serial number and click Submit. The `Free Demo License` data of the board will show on the webpage.

### Integrating the Free Demo License to the Board

1. `Copy & Paste` the `Free Demo License` data into the `CybLicense.h` file under the example sketch folder.
2. `Verify & Upload` the example sketch to the board.
3. The voice recognition is ready to go.

### Customizing a Keyword Model
Users can create their own custom keyword sets through the DSpotter Model Configuration Page:

* https://tool.cyberon.com.tw/ArduinoDSpotterAuth/CTMain.php

and obtain the `Custom Trial Model` & the corresponding `Custom Trial License` to test on the board.

1. Go to the [DSpotter Model Configuration Page](https://tool.cyberon.com.tw/ArduinoDSpotterAuth/CTMain.php).
2. Select the `board type` of your board, enter your `email address` and enter the `board's serial number`.
3. Read & agree to the end-user license agreement, then click `Next`.
4. You can choose to `create a new model` or `import model` from an existing `.dsproj` file.
5. To create a new model, select the desired language for the model and click `Create`.
6. You can set the trigger & command keyword simply by `text` input. The `keyword ID` will be used in the sketch to identify the speech recognition results.
7. After completing trigger keyword configuration, click `Next` and proceed to command keyword configuration.
8. If all the configurations in the review project page are correct, click `Confirm`. The `Custom Trial Model`, the corresponding `Custom Trial License`, and the `.dsproj file` will be sent to your email.
9. For example, you may receive files like:
   * Custom Trial Model &ensp;&ensp;&ensp;&ensp;&ensp;&ensp; ***Model_xxxxxxxxx.h***
   * Custom Trial License &ensp;&ensp;&ensp;&ensp;&ensp; ***CybLicense_xxxxxxxxx.h***
   * DSpotter Project File &ensp;&ensp;&ensp;&ensp;&ensp; ***Model_xxxxxxxxx_Arduino_[board type].dsproj***

    ***xxxxxxxxx** is a random tamp number use to differ each download. 
10. `Copy & Paste` ***Model_xxxxxxxxx.h*** and ***CybLicense_xxxxxxxxx.h*** to the sketch folder.
11. Modify the following lines in the sketch:

    ```diff
    - #include "CybLicense.h" 
    + #include "CybLicense_xxxxxxxxxxx.h"
    - #include "Model_L1.h"
    + #include "Model_xxxxxxxxxxx.h"
    ```

12. `Verify & Upload` the sketch to the board.
13. The voice recognition with your custom keyword set is ready to go.

Please note that the `Custom Trial Model` and the `Custom Trial License` are free to use with some **limitations**:

1. Recognize **50** times each reboot. Once the number of recognitions is reached, the model will **stop** until the next reboot.
2.  There is a **20-second delay** between entering the trigger mode and starting to recognize the trigger mode keywords.

### To Unlock the Limitations of the Custom Trial Version:

Users can remove the **limitations** of the Custom Trial Version by upgrading to the Custom Formal Version through the DSpotter Model License Page:
* https://tool.cyberon.com.tw/ArduinoDSpotterAuth/CFMain.php

To obtain the `Custom Formal Model` & the corresponding `Custom Formal License` for the final product:

1. Purchase a valid `voucher code` from Arduino Online Store:
   * https://store-usa.arduino.cc/
2. Go to the [DSpotter Model License Page](https://tool.cyberon.com.tw/ArduinoDSpotterAuth/CFMain.php).
3. Select the `board type` of your board, then enter your `email address`, the `board's serial number`, and the `voucher code` you just purchased.
4. Import the `.dsproj file` you received with your tested `Custom Trial Model`.
5. Read & agree to the end-user license agreement, then click `Next`.
6. If all the configurations in the review project page are correct, click `Confirm`. The `Custom Formal Model` and the corresponding `Custom Formal License` will be sent to your email.
7. For example, you may receive files like:
    * Custom Formal Model &ensp;&ensp;&ensp;&ensp;&ensp;&ensp; ***Model_xxxxxxxxx.h***
    * Custom Formal License &ensp;&ensp;&ensp;&ensp;&ensp; ***CybLicense_xxxxxxxxx.h***

    xxxxxxxxx is a random tamp number use to differ each download.
8.  `Copy & Paste` ***Model_xxxxxxxxx.h*** and ***CybLicense_xxxxxxxxx.h*** to the sketch folder.
9. Modify the following lines in the sketch:
    ```diff
    - #include "CybLicense.h" 
    + #include "CybLicense_xxxxxxxxxxx.h"
    - #include "Model_L1.h"
    + #include "Model_xxxxxxxxxxx.h"
    ```
10. `Verify & Upload` the sketch to the board.
11. The voice recognition for the custom keyword set is ready and without any limitation.


## Last Modified

* Dec. 2, 2022 by Cyberon Corporation \<info@cyberon.com.tw\>
