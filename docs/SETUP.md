# Setup and usage

## Requirements

- ESP32-based Wemos D1 R32 or compatible ESP32 board with **Bluetooth Classic SPP** support.
- Arduino IDE with ESP32 board support; exact toolchain and board package versions were not supplied.
- Compatible Bluetooth Classic serial-terminal app (the report uses an Android serial Bluetooth terminal).
- LM35 sensor, an appropriately interfaced pair of input switches, a suitable motor/relay driver, and two LED indicators.

## Firmware

1. Open `firmware/temperature_airflow_controller/temperature_airflow_controller.ino` in Arduino IDE. The sketch folder and `.ino` basename match for Arduino IDE compatibility.
2. Select the correct connected ESP32 board and port.
3. Confirm the GPIO mapping, relay polarity, sensor voltage levels, and power wiring against `HARDWARE.md` before uploading or energizing any load.
4. Compile and upload. This repository has **not** independently compiled or executed the sketch against a real board.
5. Connect a compatible Bluetooth terminal to `ESP32-WROOM-32`.
6. Send a newline-terminated setpoint (e.g., `30`) or a single-letter `R`, `L`, or `F` command. See `BLUETOOTH_PROTOCOL.md` for limitations.

**Do not use the original sketch to control mains-powered heaters, fans, or unattended machinery.** The provided prototype operated with LED indicators and has unresolved safety and command-handling concerns.

## Optional source-only checks

```bash
python scripts/check_firmware.py
python -m unittest discover -s tests -v
```

These checks need Python 3 and the standard library only. They do not prove Arduino compilation or reliable hardware operation.
