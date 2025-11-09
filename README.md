# Binomial Filtering Project (C++)

This repository implements **binomial filtering** on an image (`plant.png`) using loops in C++.  

## Filters Implemented
1. Triple 3x3 binomial filter
2. Single 7x7 binomial filter
3. Separable 7x1 and 1x7 binomial filter

The program also prints the pixel value at a chosen location (default: 100,100) for evaluation.

## How to Build and Run

Make sure OpenCV 4.x is installed. Then run:

```bash
# Create a build folder
mkdir build
cd build

# Run CMake
cmake ..

# Compile the project
make

# Run the executable
./binomial_filter
