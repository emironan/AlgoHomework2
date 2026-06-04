# AlgoHomework2

## Build and Run

### Build

To build the project, simply run:

```bash
make
```

This will compile the source files and generate the executable named `app`.

### Run

The application expects two command-line arguments:

```bash
./app <input_file> <output_file>
```

Example:

```bash
./app files/test-input-1-tsptw.txt files/test-output-1-tsptw.txt
```

### Running with Make

A convenience target is provided in the Makefile:

```bash
make run
```

By default, this command uses:

```makefile
INPUT  ?= files/test-input-1-tsptw.txt
OUTPUT ?= files/test-output-1-tsptw.txt
```

which is equivalent to:

```bash
./app files/test-input-1-tsptw.txt files/test-output-1-tsptw.txt
```

You can also specify custom input and output files:

```bash
make run INPUT=files/test-input-2-tsptw.txt OUTPUT=files/test-output-2-tsptw.txt
```
