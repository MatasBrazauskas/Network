# Network Speed Tester

A C program that tests download and upload speeds on network servers using libcurl.

## Build

```bash
make
```

This creates an executable named `main`.

## Usage

```bash
./main [OPTIONS]
```

### Options

- **`-d a`** – Download test on all servers
- **`-d <ID>`** – Download test on a single server (specify server ID)
- **`-u a`** – Upload test on all servers  
- **`-u <ID>`** – Upload test on a single server (specify server ID)
- **`-l <COUNTRY>`** – Search for best server in a specific country
- **`-c`** – Get your current location

### Examples

```bash
# Test download and upload on all servers (default)
./main

# Download test on server with ID 5
./main -d 5

# Upload test on all servers
./main -u a

# Find best server in Poland
./main -l Poland

# Get current location
./main -c

# Download on server 3 and upload on all servers
./main -d 3 -u a
```

## Dependencies

- `libcurl` – For HTTP requests
- `cjson` – For JSON parsing

## Clean

```bash
make clean
```
