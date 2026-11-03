# Complete the ESP32 project import

The verified 21-file package contains the original Arduino firmware, six prototype photographs, project documentation, and six static firmware checks. It is **not** the backup ZIP of the original PDFs.

## One-time ZIP upload

1. Download `esp32-wireless-temperature-airflow-control-git-ready.zip` from the ChatGPT conversation.
2. Open the [import branch](https://github.com/Arian-Abdollahi/esp32-wireless-temperature-airflow-control/tree/import/esp32-temperature-airflow).
3. Click **Add file → Upload files**.
4. Upload **the ZIP itself** (do not extract it), placing it in the **root** of this branch.
5. At the bottom, commit the upload **to `import/esp32-temperature-airflow`**, not `main`.
6. Visit [GitHub Actions](https://github.com/Arian-Abdollahi/esp32-wireless-temperature-airflow-control/actions) and open **Import verified ESP32 project**.
7. The workflow checks SHA256, extracts the 21-file package, preserves the preinstalled CI workflow, verifies firmware source and runs six tests, removes the temporary ZIP, then commits all files to the import branch.
8. Review the [Pull Request](https://github.com/Arian-Abdollahi/esp32-wireless-temperature-airflow-control/pulls) and the final **Static firmware checks** results before merging.

The archive has SHA256 `732204f9da7dc61dd4c44e46d54a9fa85313c5e9209407d5102557c3c2897053`. You may rename the uploaded ZIP, but must not modify its contents.

## Scope

The source implements Bluetooth **Classic SPP** connectivity, not Wi-Fi or internet cloud functionality. LED outputs stand in for a heater and fan. No live hardware build, relay safety test, or temperature calibration has been performed as part of this Git import.

Do not treat static source checks as hardware validation. Confirm wiring, voltage levels, motor electrical protections and safe operation before physical use.
