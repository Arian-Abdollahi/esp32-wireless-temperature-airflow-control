# Hardware, GPIO assignments, and electrical considerations

## Reported prototype components

- Wemos D1 R32, an ESP32-compatible development board supporting Bluetooth Classic Serial.
- LM35 analog temperature sensor.
- Two LEDs and current-limiting resistors representing heater and fan outputs.
- Two-channel relay module for bidirectional DC motor actuation.
- Two limit switches at the two movement extremes.
- Android serial Bluetooth terminal for commands and temperature telemetry.

The provided report includes prototype photos and relay example imagery; the precise wiring diagram is **not independently verified** from the photos.

## GPIO mapping (copied from the supplied sketch)

| Identifier | ESP32 GPIO | Role | Direction |
| --- | ---: | --- | --- |
| `lm35Pin` | 34 | Analog temperature sensor | Input (ADC) |
| `heaterPin` | 17 | Heater-state LED / output | Output |
| `fanPin` | 26 | Fan-state LED / output | Output |
| `relay1Pin` | 27 | Relay channel 1 | Output |
| `relay2Pin` | 14 | Relay channel 2 | Output |
| `limitSwitch1Pin` | 25 | End-stop switch 1 | Input |
| `limitSwitch2Pin` | 16 | End-stop switch 2 | Input |

## Important wiring concerns

- ESP32 GPIOs operate at **3.3 V logic levels** and are **not 5 V-tolerant**. Do not connect a 5 V sensor output directly to ADC input GPIO34; check the actual sensor output range, grounding, wiring, and analog front end. Some reports of sensors powered at 5 V do **not** establish that their outputs are safe at the ESP32 input.
- The source initializes the limit switch pins as `INPUT` and writes them `LOW`; this does not guarantee a stable logic level for an open switch. Use properly designed external pull resistors or confirmed, supported input modes.
- A two-channel relay board needs suitable voltage/current drive, suitable load supply and flyback/transient handling. The control outputs and relay polarity (active-HIGH vs active-LOW) must match the actual hardware.
- Motor and power domains should be designed with appropriate isolation/protection; do not power a motor directly from an MCU GPIO.
- The source drives relay outputs for short timed pulses (50 ms or 70 ms); actual motor travel depends on the motor/geartrain, not merely on the firmware's `H` variable. The sketch has no measured angular position feedback.
- The original demonstration used LED indicators in place of actual heater and fan hardware. Do not substitute mains-powered heating or fan equipment without safety engineering and independent testing.

See [review and safety limitations](REVIEW.md).
