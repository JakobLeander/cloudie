# cloudie

Cloudie connects an AI assistant to a small device: the cloud service accepts
mood changes through MCP, and the device polls an HTTP API to retrieve the
current state.

## Repository layout

```text
cloudie/
├── .github/workflows/       # CI workflows
├── contracts/               # State and API contracts shared across components
├── docs/                    # Architecture and project documentation
├── firmware/                # ESP firmware, display, animations, and polling
├── infra/                   # Deployment and infrastructure as code
├── services/
│   └── cloud/                # Python MCP server and device-facing HTTP API
│       ├── src/cloudie/      # Installable Python package
│       └── tests/            # Cloud service tests
└── spikes/                  # Time-boxed technical investigations
```

The firmware board and SDK are not chosen yet. Keep hardware-specific setup in
`firmware/` and record decisions in `docs/`.

## Development

The Python cloud service will be developed under `services/cloud/`. Its setup
instructions will be added there when the initial service scaffold is in place.

## Spikes

Create one subdirectory per investigation under `spikes/`. Record the question,
approach, findings, and resulting decision in that spike's README.
