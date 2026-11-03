# Bluetooth interface

The firmware starts classic `BluetoothSerial` with advertised device name **`ESP32-WROOM-32`**. This is **Bluetooth Classic SPP**, not BLE or a Wi-Fi/cloud protocol.

The commands below are inferred directly from `loop()` in the original sketch. Actual message delivery depends on the terminal newline setting and the sketch's receive-loop behavior.

| Input | Intended effect | Caveat |
| --- | --- | --- |
| `30` + newline | Set target temperature to 30°C | Only messages whose **first** byte is a digit are parsed as setpoints |
| `R` | Request rightward motor movement and heating indicator | May be overridden later in the same loop by end-stop/temperature logic |
| `L` | Request leftward motor movement and fan indicator | May be overridden later in the same loop by end-stop/temperature logic |
| `F` | Stop command (`finish = 0`); disables LED outputs at that point | **Not a proven safe shutdown**, subsequent processing can re-enable indicators in the same iteration |

For numeric input, the code builds `String(receivedChar) + SerialBT.readStringUntil('\n')`, then calls `toFloat()`. A newline-terminated nonnegative numeric command is therefore intended. The implementation does not validate a permitted temperature range or the full payload before conversion.

A serial-echo block earlier in the same loop also reads `SerialBT` bytes. As a result a pending command **may be consumed** before it reaches the command parser. A robust firmware revision should separate command reading, framing, and dispatch in a single place.

**Telemetry:** When `setpoint != 0`, the sketch sends messages shaped like `Temperature: 29.76 C`, with other diagnostic messages such as `Set Point Updated: ...` and `turn Right Updated`.

**No wireless security model is supplied.** Device pairing, authorization, message validation, and replay-resilience need assessment before use beyond a demonstration.
