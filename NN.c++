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

struct Layer{
    Matrix *weights;
    Matrix *bias;
    std::string activation;
};

class NeuralNetwork{
    public:
    Layer **layers;
    std::string f_intern;
    std::string f_extern;
    int n_layers;
    int n_neurons;
    int n_inputs;
    int n_output;

    NeuralNetwork(int N_layers, int N_neurons, int N_inputs, int N_output, std::string F_intern, std::string F_extern, float min=-1, float max=1): n_layers(N_layers), n_inputs(N_inputs), n_output(N_output), n_neurons(N_neurons), f_intern(F_intern), f_extern(F_extern){
        layers = new Layer*[n_layers];
        int prev_neurons = n_inputs;

        for(int i=0; i<n_layers - 1; i++){
            layers[i] = createLayer(f_intern, n_neurons, prev_neurons, min, max);
            prev_neurons = n_neurons;
        }

        layers[n_layers - 1] = createLayer(f_extern, n_output, prev_neurons, min, max);
    };

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

    ~NeuralNetwork() {
        for (int i = 0; i < n_layers; i++) {
            delete layers[i]->weights;
            delete layers[i]->bias;
            delete layers[i];
        }
        delete[] layers;
    };

    private:
    float fns(std::string activation, float x){
        if(activation == LINEAR) return x;
        if(activation == TANH) return tanh(x);
        if(activation == SIGMOID) return 1 / (1 + exp(-x));
        if(activation == RELU) return (x > 0) ? x : 0;
        return 0;
    };

    Matrix *act(std::string activation, Matrix *x){
        for(int i=0; i<x->n*x->m; i++) x->matrix[i] = fns(activation, x->matrix[i]);
        return x;
    }

    Layer *createLayer(std::string f_activation, int n, int m, float min, float max){
        Layer *layer = new Layer;
        layer->weights = Matrix::randomMatrix(n, m, min, max);
        layer->bias = Matrix::randomMatrix(n,1,min,max);
        layer->activation = f_activation;

        return layer;
    };

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
    
    float xor_data[] = {0,0,0,1,1,0,1,1};
    float xor_labels[] = {0,1,1,0};
    float input[] = {1, 0};

    Matrix *Input = new Matrix(input, 2, 1);
    Matrix *Xor_data = new Matrix(xor_data, 4, 2);
    Matrix *Xor_labels = new Matrix(xor_labels, 4, 1);


    std::unique_ptr<NeuralNetwork> NN = std::make_unique<NeuralNetwork>(64,32,2,1,SIGMOID,TANH);

    return 0;
}