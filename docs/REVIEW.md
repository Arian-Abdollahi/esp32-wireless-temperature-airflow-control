# Technical review and known limitations

The repository preserves the supplied prototype firmware; it does not claim a production-ready safety design.

## Findings requiring engineering attention

1. **Bluetooth command loss:** the early `SerialBT.available()` echo section uses `SerialBT.read()` before the later command handler does. A command may disappear before parsing.
2. **Stop semantics:** the `F` branch clears LED outputs and sets `finish = 0`, but the rest of the same loop can still set outputs based on switches or temperature. Afterward the entire loop is skipped. It is not a certified emergency stop.
3. **Limit switch logic:** the two limit switch inputs are initialized with `INPUT`; no stable pull-resistor behavior is demonstrated, and the code can issue movement while a switch is asserted. Verify switch polarity, direction, and travel-stop behavior before applying motor power.
4. **ADC conversion:** `sensorValue * 0.489 - 80` is undocumented for this specific ESP32 ADC and LM35 circuit. Validate against calibrated temperatures; consider ADC attenuation, nonlinearities, noise and supply voltage.
5. **Unvalidated temperature setpoints:** numeric strings are converted to `float` without bounds or range checks. A negative setpoint command is not parsed as a setpoint since `'-'` fails `isdigit()`.
6. **No fail-safe heater control:** no watchdog, sensor disconnection check, over-temperature interlock, emergency disconnect, or relay stuck-on detection is established.
7. **Motor timing:** `delay(50)` or `delay(70)` relay pulses and `H` states do not imply calibrated 90-degree angular positioning.
8. **Main-loop delay:** `delay(1000)` introduces approximately one second of control latency, in addition to other delays. Report text and inline comment contain inconsistent delay descriptions.
9. **Prototype vs deployment:** documented heater/fan functions use LEDs. No evidence of a safe mains-powered heater or integrated air-control appliance is provided.
10. **Wireless terminology:** Bluetooth Classic SPP wireless communication is implemented. Wi-Fi, an IP network, MQTT, cloud APIs, logging backend and internet connectivity are **not** implemented, so avoid claiming those as shipped features.

## Potential next iteration (not included in preserved source)

- Separate input parsing from telemetry echo using a line-buffered, bounded command parser.
- Define explicit operating modes and latched fault/stop states.
- Prioritize motor end-stop protection over all remote requests and use hardware interlocks where appropriate.
- Calibrate temperature sensing with known reference points and range checks.
- Make output states fail-safe by default; add timeouts and watchdogs.
- Bench test with low-voltage/current-limited loads before considering any real heating or powered fan hardware.

## Verification performed

- Confirmed source syntax text, GPIO mapping, Bluetooth SPP feature gates, control functions and command tokens from the original `.ino` file.
- Cross-checked stated hardware and behavior with the supplied technical report and saved prototype photos.
- Ran static source integrity and repository utility tests.
- **Not performed:** ESP32 flashing, Arduino compilation, real Bluetooth connectivity, temperature calibration, electrical tests, actuator tests, or acceptance testing.
