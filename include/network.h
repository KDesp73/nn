#ifndef NETWORK_H
#define NETWORK_H

#include "lists.h"
#include <stdlib.h>
#include "config.h"

/**
 * @struct Neuron
 * Represents a single neuron in the neural network.
 * 
 * @field value The output value of the neuron.
 * @field weights The weights associated with connections to the next layer.
 * @field delta The error term used during backpropagation.
 * @field bias The bias term for the neuron.
 */
typedef struct {
    double value;
    Doubles weights; // Forward
    double delta;
    double bias;
} Neuron;

/**
 * Initializes a Neuron with a given value and weights.
 * 
 * @param value The initial value of the neuron.
 * @param weights The weights for the neuron's connections.
 * @return A fully initialized Neuron structure.
 */
Neuron NeuronInit(double value, Doubles weights);

/**
 * Frees the resources associated with a Neuron.
 * 
 * @param neuron Pointer to the Neuron to be freed.
 */
void NeuronFree(Neuron* neuron);

/**
 * @struct Layer
 * Represents a single layer in the neural network.
 * 
 * @field neuronCount The number of neurons (outputs) in the layer.
 * @field neurons Pointer to an array of neurons in the layer.
 */
typedef struct {
    size_t neuronCount;  // Number of outputs (neurons) in this layer
    Neuron* neurons;
} Layer;

/**
 * Initializes a Layer with a specified number of inputs and outputs.
 * 
 * @param layer Pointer to the Layer to initialize.
 * @param inputCount Number of inputs to the layer.
 * @param outputCount Number of neurons (outputs) in the layer.
 */
void LayerInit(Layer* layer, size_t inputCount, size_t outputCount);

/**
 * Frees the resources associated with a Layer.
 * 
 * @param layer Pointer to the Layer to be freed.
 */
void LayerFree(Layer* layer);

/**
 * @struct Network
 * Represents the entire neural network.
 * 
 * @field layerCount The number of layers in the network.
 * @field layers Array of layers in the network. Size is limited by MAX_LAYERS.
 * @field totalLoss Tracks the total loss/error of the network.
 */
typedef struct {
    size_t layerCount;
    Layer layers[MAX_LAYERS];
    double totalLoss;
} Network;

/**
 * Initializes a Network with specified layer sizes.
 * 
 * @param network Pointer to the Network to initialize.
 * @param sizes A dynamic array specifying the number of neurons in each layer.
 */
void NetworkInit(Network* network, const Sizes sizes);

/**
 * Frees the resources associated with a Network.
 * 
 * @param network Pointer to the Network to be freed.
 */
void NetworkFree(Network* network);

/**
 * Prints the details of a Network.
 * 
 * @param network Pointer to the Network to print.
 */
void NetworkPrint(const Network* network);

/**
 * Loads a Network from a file.
 * 
 * @param network Pointer to the Network to load data into.
 * @param file Path to the file containing network data.
 */
void NetworkLoad(Network* network, const char* file);

/**
 * Saves a Network to a file.
 * 
 * @param network Pointer to the Network to save.
 * @param file Path to the file to save the network data.
 */
void NetworkSave(const Network* network, const char* file);

#endif // NETWORK_H
