# nn

Neural network library and interactive training interface, written in pure C.

```
make all
```

## Library

Header-only API in [`include/`](./include):

| Header | Description |
|--------|-------------|
| `network.h` | Neuron, Layer, Network — core structures and init/free/load/save |
| `activation.h` | Sigmoid, ReLU, Tanh activation functions |
| `methods.h` | Forward pass, backpropagation, training loop |
| `config.h` | Config struct, defaults, setters, file loading |
| `data.h` | TrainingData struct — load, free, print, validate |
| `lists.h` | Generic dynamic arrays — `Sizes`, `Doubles`, `Doubles2D` |
| `misc.h` | Utilities — `randomFloat()`, `DOUBLE_PRINT` |

### Quick example

```c
#include "network.h"
#include "methods.h"
#include "data.h"
#include "config.h"

int main() {
    CONFIG_INIT();
    ConfigLoad("nn.conf");

    TrainingData data;
    TrainingDataLoad(&data, "data/xor.dat");

    Network n;
    NetworkInit(&n, SizesInit(data.inputCount, 3, data.outputCount, SIZESEND));
    TrainNetwork(&n, &data, config.epochs);
    NetworkSave(&n, "trained.sav");

    NetworkFree(&n);
    TrainingDataFree(&data);
}
```

### Configuration

Loaded from `nn.conf`:

```
epochs=50000
activation=SIGMOID
learningRate=1.8
targetLoss=0.005
```

Supports: `learningRate`, `minLearningRate`, `learningRateDecay`, `theta`, `maxGradient`, `activation` (SIGMOID/RELU/TANH), `epochs`, `biasInit`, `biasEngage`, `targetLoss`.

## REPL (`nni`)

Interactive CLI for training and evaluating networks.

```
./repl/nni
```

```
> nn 2 3 1
> load data data/xor.dat
> train
> feed 1 0
0.9876
```

| Command | Description |
|---------|-------------|
| `nn <LAYERS>...` | Create a new network (e.g. `nn 2 8 1`) |
| `load data <PATH>` | Load training data |
| `load nn <PATH>` | Load a saved network |
| `save <PATH>` | Save the current network |
| `train` | Train the network |
| `feed <VALUES>...` | Run inference |
| `set <KEY> <VALUE>` | Change a config option |
| `print nn` / `data` / `config` / `state` | Print information |
| `clear` | Clear screen |
| `help` | Show commands |
| `quit` | Exit |

## Build

```
make all          # library (.a, .so) + nn executable + repl/nni
make static       # libnn.a only
make shared       # libnn.so only
make clean        # remove build artifacts
make install      # install library system-wide
```

## References

- [GSL - GNU Scientific Library](https://www.gnu.org/software/gsl/)
- [neuralnetworksanddeeplearning](http://neuralnetworksanddeeplearning.com/chap1.html)
- [Theory](https://theneuralblog.com/forward-pass-backpropagation-example/)

## License

[MIT](./LICENSE)
