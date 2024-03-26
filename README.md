# All-In-One Card Project

## Introduction
The All-In-One Card Project is a device created using ESP32 and an OLED display that allows users to display images of different cards uploaded to the device. This eliminates the need to carry multiple physical cards every day, providing convenience and efficiency.

## Example

![Card](Product.jpg)

## Features
- Display images of different cards on demand.
- Wireless control via Wi-Fi connection.
- Simple web interface for easy operation.
- Supports multiple card types.
- Compact and portable design.

## Hardware Requirements
- ESP32 development board
- OLED display (128x64 pixels)
- Push button
- Wi-Fi network for wireless connectivity

## Software Requirements
- Arduino IDE
- Required libraries:
  - ezButton
  - U8g2lib
  - WiFi

## Installation and Setup
1. Connect the ESP32, OLED display, and push button according to the circuit diagram.
2. Install the necessary libraries in the Arduino IDE.
3. Upload the provided Arduino sketch (`main.ino`) to the ESP32 board.
4. Modify the sketch to include the byte arrays of your desired card images.
5. Update the Wi-Fi credentials in the sketch.
6. Compile and upload the sketch to the ESP32 board.
7. Power on the device and connect it to your Wi-Fi network.
8. Access the web interface by entering the IP address of the device in a web browser.

## Usage
1. Upon accessing the web interface, you'll see options to display different cards.
2. Click on the "Next" button to cycle through the available cards.
3. Click on the "Previous" button to go back to the previous card.
4. Optionally, click on the "RESET" button to reset the device.
5. Alternatively, use the physical push button to switch between cards.

## Customization
- Add or remove card images by modifying the sketch and updating the byte arrays.
- Customize the web interface by editing the HTML and CSS code in the sketch.
- Adjust the functionality of the device according to your specific requirements.

## Contributors
- [Rohitashva](https://github.com/rohitashvakumawat) - Creator and maintainer

## License
This project is licensed under the [MIT License](LICENSE).
