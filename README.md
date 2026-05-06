# Network Speed Tester

A C program that tests download and upload speeds on network servers using libcurl.

## Build

```bash
make
```

This creates an executable named `main`.

## Usage

```bash
./bin/main [OPTIONS]
```

## Clean

```bash
make clean
```

### Options

- **`-d a`** – Download test on all servers
- **`-d <ID>`** – Download test on a single server (specify server ID)
- **`-u a`** – Upload test on all servers  
- **`-u <ID>`** – Upload test on a single server (specify server ID)
- **`-l <COUNTRY>`** – Test servers in a specific country, print their results, and report the best upload/download server
- **`-c`** – Get your current location

### Examples

```bash
# Test download and upload on all servers (default)
./bin/main

# Download test on server with ID 5
./bin/main -d 5

# Upload test on all servers
./bin/main -u a

# Find best server in Poland
./bin/main -l Poland

# Get current location
./bin/main -c

# Download on server 3 and upload on all servers
./bin/main -d 3 -u a
```

## Dependencies

- `libcurl` – For HTTP requests
- `cjson` – For JSON parsing
