# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).


## [Unreleased]

### Added

- REPL (old `nni` project)

### Fixed

- network.h: Removed const from Sizes parameter in NetworkInit declaration 
- activation.c: StringToActivation default return changed from -1 to ACT_SIGMOID
- data.c: Fixed null-check (!file → !f); removed pipe-delimiter parsing logic
- layer.c: int loop variables → size_t; moved bias init inside loop
- methods.c: Cost now returns proper quadratic cost 0.5 * (target - predicted)^2 (was just diff)
- methods.c: CostDerivative returns predicted - target (was incorrect)
- methods.c: Added gradient clamping (clampGradient) to prevent exploding gradients
- methods.c: Layer iteration: int → size_t, pointer-based access instead of copies
- methods.c: Hidden-layer backprop loop rewrite — fixed off-by-one indexing and delta computation
- methods.c: Removed malloc/free of errors array in training loop (unused)
- methods.c: Removed dead code/commented printf
- network.c: Added null-checks and validation in NetworkLoad for layer count, token parsing

## [0.0.2] - 2025-01-05 

### Added

- Documentation in header files

### Fixed

- Using config.theta instead of THETA macro
- Using config.activation instead of ACTIVATION macro


## [0.0.1] - 2025-01-03 

### Added

- Added network structure
- Forward pass
- Backpropagation
- Load / Save network methods added
- Loading training data from file

[0.0.2]: https://github.com/KDesp73/nn/releases/tag/v0.0.2
[0.0.1]: https://github.com/KDesp73/nn/releases/tag/v0.0.1
