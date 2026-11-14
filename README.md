# Cloudie

Cloudie is a project to build an animated AI companion that appears to float
inside a small display cube. An ESP drives an OLED, whose image is reflected by
a beamsplitter to create a Pepper's ghost illusion. When you interact with
Claude, Cloudie's expression and animation will change to reflect the
companion's current mood or activity.

This is a Pepper's ghost optical illusion, not a true hologram.

## How it will work

1. Claude sends a mood or activity update through a cloud service.
2. The ESP retrieves the companion's current state over HTTP.
3. The ESP renders the corresponding animation on the OLED.
4. The beamsplitter reflects the OLED image into the cube, making the
   companion appear to float inside it.

## Project status

The repository is an early scaffold. The cloud service is planned to expose an
MCP tool for updating the companion's state and an HTTP API for the device.
The firmware, state contract, and deployment details are still under
development.

## Hardware

For prototyping, the device uses an ESP32-WROOM-32 DevKit V1. The display is a
Waveshare [1.5-inch RGB OLED module](https://www.waveshare.com/product/displays/oled/oled-3/1.5inch-rgb-oled-module.htm)
(128×128 resolution, 65K colors, SPI interface, 1351 driver). Its viewing area is
26.86 × 26.86 mm and the module measures about 44.5 × 37.0 mm. This is the largest square quality micro OLED I cound find.

The OLED is connected to the ESP32 DevKit V1 using the following pins:

| OLED pin | ESP32 DevKit pin | Notes |
| --- | --- | --- |
| VCC | 3V3 | The module supports a 3.3 V or 5 V supply, but its supply and logic voltage must match. Use 3V3 for the ESP32's 3.3 V logic. |
| GND | GND | |
| DIN | GPIO23 | MOSI (VSPI) |
| CLK (SCL) | GPIO18 | SCK (VSPI) |
| CS | GPIO5 | Any free GPIO can be used. |
| DC | GPIO16 | Any free GPIO can be used. |
| RST | GPIO17 | Any free GPIO can be used. |

## Repository layout

```text
cloudie/
├── .github/workflows/       # CI workflows
├── docs/                    # Architecture and project documentation
├── firmware/                # ESP firmware, display, animations, and polling
├── infra/                   # Deployment and infrastructure as code
├── services/
│   └── cloud/               # Planned MCP server and device-facing HTTP API
└── spikes/                  # Time-boxed technical investigations
```

## Development

The cloud service is being developed under `services/cloud/`; see its
[README](services/cloud/README.md) for its current design and open decisions.
Firmware setup and hardware-specific choices will be documented under
`firmware/` and `docs/` as they are made.

## Spikes

Create one subdirectory per investigation under `spikes/`. Record the
question, approach, findings, and resulting decision in that spike's README.
