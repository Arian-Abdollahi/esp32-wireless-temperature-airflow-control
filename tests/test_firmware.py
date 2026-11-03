import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'scripts'))
from check_firmware import SOURCE, EXPECTED_PINS, extract_pins, check_content


class FirmwareIntegrityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.content = SOURCE.read_text(encoding='utf-8')

    def test_source_is_present(self):
        self.assertGreater(SOURCE.stat().st_size, 1000)

    def test_pin_mapping(self):
        self.assertEqual(extract_pins(self.content), EXPECTED_PINS)

    def test_gpio_uniqueness(self):
        self.assertEqual(len(set(EXPECTED_PINS.values())), len(EXPECTED_PINS))

    def test_protocol_and_motor_functions(self):
        self.assertEqual(check_content(self.content), [])

    def test_bluetooth_classic_feature_guard(self):
        self.assertIn('CONFIG_BT_SPP_ENABLED', self.content)

    def test_led_and_motor_outputs_present(self):
        for pin in ('heaterPin', 'fanPin', 'relay1Pin', 'relay2Pin'):
            self.assertIn(f'pinMode({pin}, OUTPUT)', self.content)


if __name__ == '__main__':
    unittest.main()
