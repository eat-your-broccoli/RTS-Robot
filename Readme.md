# RTS-Robot

This project is used for the [elegoo smart robot kit v4.0](https://www.elegoo.com/collections/robot-kits/products/elegoo-smart-robot-car-kit-v-4-0). 

[`Platform.IO`](https://platformio.org/) is used for development.

## Notes

### Chip Version

This robot has a TB6612-chip. If this project does not work on your robot, check the microcontroller.

## Preparation

### Installation

1. Install [Visual Studio Code](https://code.visualstudio.com/).

2. Install the [Arduino Plugin](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino)

3. Install [Platform.IO Plugin](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)

4. Restart VS Code

5. To confirm that installation was successful, the blue bar at the bottom should display new icons. We're interested in the 🏠, ✔ and ➡ buttons.

### Import project in Platform.IO

1. click on the 🏠 button at the blue bar at the bottom. Platform.IO Home screen opens.

2. click on "Open Project". A file explorer opens.

3. navigate into "RTS-Robot/tamagotchi"

4. click on "open tamagotchi"

5. Platform.IO should now download and install necessary dependencies.

### Libraries

`NOTE: THIS IS ONLY RELEVANT IF YOU CHOOSE TO USE THE ARDUINO IDE`

The project relies on libraries to function. These libraries are in the directory `CopyMeFirst`.

Instructions to add those libraries to your Arduino IDE can be found here: `CopyMeFirst/AddLibraries.pdf`.

For `Platform.IO`, these libraries are already added to the project under `lib`. 

## Building

Hit the ✔ button.

## Flashing
 
Flash the software to your board.

1. Make sure that the board is connected to your computer

2. Make sure that the shield's lever is set to "upload".

3. That's it. `Platform.IO` detects automatically the port of your board. (To manually select a port, see `devices` section in the menu)

4. Hit the ➡ button to flash.
