//
// Created by GS <sgazeos@gmail.com>
//


#include <ops/declarable/headers/parity_ops.h>
#include <ops/declarable/helpers/dropout.h>

namespace nd4j {
namespace ops {


//////////////////////////////////////////////////////////////////////////
CONFIGURABLE_OP_IMPL(dropout, 1, 1, true, 1, 1) {
    NDArray<T>* input   = INPUT_VARIABLE(0); // lookup param

    NDArray<T>* reduceShape = nullptr; // this param is optional
    NDArray<T>* output  = OUTPUT_VARIABLE(0); // 
    
    int seed = INT_ARG(0);
    
    T probValue = T_ARG(0); 
    if (block.width() > 1)
        reduceShape = INPUT_VARIABLE(1);

    REQUIRE_TRUE(probValue > T(0.f) && probValue <= T(1.f), 0, "dropout: Probability should be with range 0 to 1.");

    if (probValue == T(1.0)) {
        *output = *input;
        return ND4J_STATUS_OK;
    }
    nd4j::random::RandomBuffer* rng = block.getRNG();
    
    if (rng == nullptr)
        return ND4J_STATUS_BAD_RNG;

    return helpers::dropOutFunctor(rng, input, output, reduceShape, seed, probValue);
}

}
}