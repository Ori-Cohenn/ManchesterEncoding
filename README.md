# Manchester Encoding with Adafruit Feather M0 LoRa

This project demonstrates Manchester Encoding using two Adafruit Feather M0 LoRa microcontroller units (MCUs). The data flows over the GPIO lines, establishing a master and slave relationship.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Code Overview](#code-overview)
- [Contributing](#contributing)
- [License](#license)

## Introduction

Manchester Encoding is a method of encoding digital data where each bit of data is represented by at least one transition. This project uses two Adafruit Feather M0 LoRa MCUs to implement Manchester Encoding over GPIO lines, with one MCU acting as the master and the other as the slave.

## Features

- **Manchester Encoding**: Robust data transmission with built-in synchronization.
- **Master-Slave Configuration**: One MCU sends data (master), while the other receives (slave).
- **GPIO Communication**: Data transfer via GPIO lines.

## Hardware Requirements

- 2 x Adafruit Feather M0 LoRa MCUs
- Jumper wires
- Breadboard (optional)

## Software Requirements

- [PlatformIO](https://platformio.org/)
- [Adafruit SAMD Boards](https://github.com/adafruit/ArduinoCore-samd)

## Installation

1. **Clone the repository**:
    ```bash
    git clone https://github.com/Ori-Cohenn/ManchesterEncoding.git
    cd ManchesterEncoding
    ```

2. **Open the project in PlatformIO**.

3. **Install the necessary libraries**:
    PlatformIO should automatically install the required libraries listed in `platformio.ini`.

## Usage

1. **Wiring**:
    - Connect the GPIO pin of the master MCU to the GPIO pin of the slave MCU.
    - Ensure both MCUs share a common ground.

2. **Upload Code**:
    - Open the `master` folder in PlatformIO and upload the code to the master Feather M0.
    - Open the `slave` folder in PlatformIO and upload the code to the slave Feather M0.

3. **Run the Project**:
    - Power both MCUs. The master will start transmitting data encoded in Manchester format, and the slave will decode and display the received data.

## Code Overview

### Master

The master MCU encodes data using Manchester encoding and sends it over a GPIO line.

### Slave

The slave MCU receives Manchester encoded data from the master, decodes it, and processes it accordingly.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
