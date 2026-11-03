# Temperature and motor control behavior

The original sketch combines remote commands, two limit switch states, and temperature thresholds in one `loop()` function. This is a documentation of existing code, not a redesign or verification of its safety.

## Thermal thresholds

- `deadband = 5.0`, `setpoint = 0.0` initially.
- Below `setpoint - 5°C`, if `setpoint > 0`, the sketch enables the heating indicator and calls `turn_right()`.
- Above `setpoint + 5°C`, if `setpoint > 0`, it enables the fan indicator and calls `turn_left()`.
- Otherwise it switches off both indicators and attempts to return the motor toward its neutral state depending on `H`.

Because each boundary is separated from the setpoint by 5 degrees, the overall neutral temperature span is **10°C**. No independent hysteresis memory variable is implemented for heat/cool toggling.

## Movement tracking

The integer `H` is the firmware's abstract state variable:

| Value | Meaning described by report |
| ---: | --- |
| `0` | Initial/neutral |
| `-1` | Rightward state |
| `1` | Leftward state |

These values are **not** physical angle measurements. `turn_left()` and `turn_right()` pulse relay outputs, then update `H`. At the end of every `loop()` pass, both relays are set to LOW.

## Effective precedence in the source

1. Any Bluetooth command recognized by the receive block is handled.
2. Limit switch inputs are evaluated in `if/else if` branches.
3. If neither limit branch activates, temperature thresholds are considered.
4. The `F` command updates `finish` but the rest of the current iteration still executes.

The report describes manual commands as having first priority. The *code*, however, can overwrite output states in later branches in the same iteration. Limit switches as coded do not unconditionally prevent dangerous travel, and the script lacks a verified hardware-independent emergency stop.

## Sensor conversion

```cpp
int sensorValue = analogRead(lm35Pin);
float temperatureC = sensorValue*0.489 - 80;
```

This is the actual source expression, not the standard nominal LM35 voltage-to-temperature conversion. The report discusses temperature sensor calibration but does not establish where these constants came from or whether the reading is accurate across the working range. Validate against actual ADC characteristics and measured reference temperatures.
