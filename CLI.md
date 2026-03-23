# LINE CLI

Command-line interface for the LINE queueing network solver.


## Usage

The CLI is a single Python script with no external dependencies (only Python 3.9+ standard library).

```bash
# Make executable (optional)
chmod +x line.py

# Run directly
python line.py <command> [options]
./line.py <command> [options]
```

### Solve a Model

```bash
# Solve with default settings (MVA solver)
python line.py solve model.jsimg

# Use a specific solver
python line.py solve model.jsimg -s fluid

# Output as JSON
python line.py solve model.jsimg -o json

# Solve with specific analysis type
python line.py solve model.jsimg -a avg  # Average metrics only
python line.py solve model.jsimg -a sys  # System metrics only

# Use a random seed for stochastic solvers
python line.py solve model.jsimg -s jmt -d 12345

# Read from stdin
cat model.jsimg | python line.py solve -i jsimg -s mva
```

### Available Solvers

- `mva` - Mean Value Analysis (default)
- `ctmc` - Continuous-Time Markov Chain
- `fluid` - Fluid/Mean-Field ODE
- `jmt` - Java Modelling Tools (simulation)
- `nc` - Normalizing Constant
- `ssa` - Stochastic Simulation Algorithm
- `ln` - Layered Network solver
- `lqns` - LQN Solver

```bash
# List all solvers
python line.py list solvers
```

### Input Formats

- `jsim` - JSIM format
- `jsimg` - JSIMG format (with graphics)
- `jsimw` - JSIMW workspace format
- `lqnx` - LQN XML format
- `xml` - Generic XML format

```bash
# List all formats
python line.py list formats
```

### System Information

```bash
# Display system info, JAR status, and solver compatibility
python line.py info
```

### Server Mode

```bash
# Start WebSocket server on default port (5863)
python line.py server

# Start on custom port
python line.py server -p 8080

# Bind to all interfaces
python line.py server -H 0.0.0.0 -p 5863
```

## Configuration

Create `~/.config/line-cli/config.yaml` (requires PyYAML):

```yaml
# Path to jline.jar (auto-detected if not specified)
# jar_path: /path/to/jline.jar

# Java executable path
java_path: java

# Default solver
default_solver: mva

# Default output format
default_output_format: table

# Server settings
server:
  host: localhost
  port: 5863
```

## Environment Variables

- `LINE_JAR_PATH` - Path to jline.jar

## Requirements

- Python 3.9+
- Java 8+ (for running the LINE solver JAR)
- jline.jar (auto-detected in `common/jline.jar` or set via `LINE_JAR_PATH`)

Optional:
- PyYAML (for configuration file support)
