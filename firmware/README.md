# Firmware

This directory contains ESP32 firmware for device startup, display and
animation code, and polling the cloud service for state.

To connect to Wi-Fi, copy `include/secrets.h.example` to `include/secrets.h`
and set `WIFI_SSID` and `WIFI_PASSWORD`. `WORKER_URL` is prefilled with the
Cloudflare Worker URL. The local credentials file is ignored by Git.
