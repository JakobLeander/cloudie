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
The firmware, hardware setup, state contract, and deployment details are still
under development. The ESP board and firmware SDK have not been finalized.

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
