#!/usr/bin/env python3
"""Non-invasive validation of the supplied Arduino sketch's structure and pin map."""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / 'firmware' / 'temperature_airflow_controller' / 'temperature_airflow_controller.ino'
EXPECTED_PINS = {
    'lm35Pin': 34, 'heaterPin': 17, 'fanPin': 26,
    'relay1Pin': 27, 'relay2Pin': 14,
    'limitSwitch1Pin': 25, 'limitSwitch2Pin': 16,
}


def extract_pins(content: str) -> dict[str, int]:
    return {name: int(number) for name, number in
            re.findall(r'\bconst\s+int\s+(\w+)\s*=\s*(\d+)\s*;', content)}


def check_content(content: str) -> list[str]:
    errors = []
    pins = extract_pins(content)
    if pins != EXPECTED_PINS:
        errors.append(f'Pin mapping differs from preserved original: {pins!r}')
    if len(set(pins.values())) != len(pins):
        errors.append('GPIO assignments are not unique')
    for token in [
        '#include "BluetoothSerial.h"',
        'BluetoothSerial SerialBT;',
        'SerialBT.begin(device_name)',
        'analogRead(lm35Pin)',
        'float deadband = 5.0',
        'void turn_right()',
        'void turn_left()',
        'limitSwitch1State',
        'limitSwitch2State',
        "receivedChar == 'R'",
        "receivedChar == 'L'",
        "receivedChar == 'F'",
        'finish = 0',
    ]:
        if token not in content:
            errors.append(f'Missing expected source token: {token}')
    return errors


def main():
    content = SOURCE.read_text(encoding='utf-8')
    errors = check_content(content)
    if errors:
        raise SystemExit('\n'.join(errors))
    print(f'ESP32 sketch structure OK. {len(EXPECTED_PINS)} GPIOs mapped; Bluetooth control tokens present.')
    print('Static check only. No compilation, calibration, safety or bench validation performed.')


if __name__ == '__main__':
    main()
