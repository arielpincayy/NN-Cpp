#include "./lib/matrix.h"
#include <string>
#include <cmath>
#include <vector>
#include <thread>
#include <memory>
#include <algorithm>

#define LINEAR "linear"
#define SIGMOID "sigmoid"
#define TANH "tanh"
#define RELU "relu"

// Structure to define a neural network layer
struct Layer{
    Matrix *weights;   // Weight matrix of the layer
    Matrix *bias;      // Bias matrix of the layer
    std::string activation; // Activation function of the layer
};

class NeuralNetwork{
    public:
    Layer **layers;      // Array of pointers to layers
    std::string f_intern; // Internal activation function
    std::string f_extern; // Output activation function
    int n_layers;        // Number of layers
    int *n_neurons;       // Number of neurons per hidden layer
    int n_inputs;        // Number of input neurons
    int n_output;        // Number of output neurons

    // Constructor to initialize the neural network
    NeuralNetwork(int N_layers, int *N_neurons, int N_inputs, int N_output, std::string F_intern, std::string F_extern, float min=-1, float max=1): n_layers(N_layers), n_inputs(N_inputs), n_output(N_output), n_neurons(N_neurons), f_intern(F_intern), f_extern(F_extern){
        //Create layers
        layers = new Layer*[n_layers+1];

        layers[0] = createLayer(f_intern, n_neurons[0], n_inputs, min, max);
        int prev_neurons = n_neurons[0];

        for(int i=0; i<n_layers - 1; i++){
            layers[i+1] = createLayer(f_intern, n_neurons[i+1], prev_neurons, min, max);
            prev_neurons = n_neurons[i+1];
        }

        layers[n_layers] = createLayer(f_extern, n_output, prev_neurons, min, max);
    };

    // Forward propagation function
    Matrix *predict(Matrix *input, bool normlzt = false){
        if(normlzt) normalization(input, n_inputs);

        for(int i=0; i<n_layers; i++){
            Matrix *frac1 = ((*layers[i]->weights) * (*input));
            Matrix *temp = act(layers[i]->activation, *frac1 + (*layers[i]->bias));
            delete frac1;
            delete input;
            input = temp;
        };

        return input;
    };

    // Destructor to free memory
    ~NeuralNetwork() {
        for (int i = 0; i < n_layers; i++) {
            delete layers[i]->weights;
            delete layers[i]->bias;
            delete layers[i];
        }
        delete[] layers;
    };

    private:
    // Activation function computation
    float fns(std::string activation, float x){
        if(activation == LINEAR) return x;
        if(activation == TANH) return tanh(x);
        if(activation == SIGMOID) return 1 / (1 + exp(-x));
        if(activation == RELU) return (x > 0) ? x : 0;
        return 0;
    };

    // Apply activation function to matrix
    Matrix *act(std::string activation, Matrix *x){
        for(int i=0; i<x->n*x->m; i++) x->matrix[i] = fns(activation, x->matrix[i]);
        return x;
    }

    // Create a layer with specified parameters
    Layer *createLayer(std::string f_activation, int n, int m, float min, float max){
        Layer *layer = new Layer;
        layer->weights = Matrix::randomMatrix(n, m, min, max);
        layer->bias = Matrix::randomMatrix(n,1,min,max);
        layer->activation = f_activation;

        return layer;
    };

    // Normalize input matrix
    void normalization(Matrix *input, int n){
        float mean = 0;
        float dstd = 0;
        for(int i=0; i<n; i++) mean += input->matrix[i];
        mean /= n;
        for(int i=0; i<n; i++) dstd += pow((input->matrix[i] - mean),2);
        dstd = sqrt(dstd/n);
        for(int i=0; i<n; i++) input->matrix[i] = (input->matrix[i] - mean)/dstd;
    }

};

int main(){
    
    float xor_data[] = {0,0,0,1,1,0,1,1}; // XOR input data
    float xor_labels[] = {0,1,1,0};       // XOR expected output
    float input[] = {1, 0};               // Example input

    Matrix *Input = new Matrix(input, 2, 1);
    Matrix *Xor_data = new Matrix(xor_data, 4, 2);
    Matrix *Xor_labels = new Matrix(xor_labels, 4, 1);

    int N_neurons[] = {16, 32, 64}; // Number of neurons per hidden layer
    int N_layers = 3;           // Number of layers

    std::unique_ptr<NeuralNetwork> NN = std::make_unique<NeuralNetwork>(N_layers, N_neurons, 2, 1, SIGMOID, SIGMOID);
    Matrix *output = NN->predict(Input, true);
    std::cout << "Output: " << *output->matrix << '\n';

    return 0;
}
