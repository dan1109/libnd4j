//
//  @author raver119@gmail.com
//

#include <ops/declarable/headers/parity_ops.h>
#include <ops/declarable/helpers/cross.h>

namespace nd4j {
namespace ops {

    OP_IMPL(weighted_cross_entropy_with_logits, 3, 1, true) {

        NDArray<T>* targets = INPUT_VARIABLE(0);
        NDArray<T>* input = INPUT_VARIABLE(1);
        NDArray<T>* weights = INPUT_VARIABLE(2);
        T posWeight;
        
        REQUIRE_TRUE(targets->isSameShape(input), 0, "weighted_cross_entropy_with_logits: The shape of both input params should be equal.");
        REQUIRE_TRUE(weights->isScalar() || targets->sizeAt(-1) == weights->lengthOf(), 0, "weighted_cross_entropy_with_logits: The weights should be scalar or vector with last dimention size");
        NDArray<T>* output = OUTPUT_VARIABLE(0);

        if (weights->isScalar()) {
            posWeight = weights->getScalar(0);
            auto mainRoutine = LAMBDA_TT(_x, _z, posWeight) {
                T targetWeight = (1 + (posWeight - 1) * _z);
                return (1 - _z) * _x + 
                    targetWeight * (nd4j::math::nd4j_log(1 + nd4j::math::nd4j_exp(-nd4j::math::nd4j_abs(_x))) + 
                    nd4j::math::nd4j_max(-_x, T(0.0))
                );
            };
            input->applyPairwiseLambda(targets, mainRoutine, output);
        }
        else
        {
            std::unique_ptr<NDArray<T>> targetVector(new NDArray<T>(*weights));
            targetVector->template applyScalar<simdOps::Add<T>>(T(-1.0));
            
            auto mainRoutine = LAMBDA_TTT(_x, _z, _w) {
                return (((T)1.0 - _z) * _x) + 
                    _w * (nd4j::math::nd4j_log(1 + nd4j::math::nd4j_exp(-nd4j::math::nd4j_abs(_x))) + 
                    nd4j::math::nd4j_max(-_x, T(0.0)));
            };

            std::unique_ptr<NDArray<T>> targetTensor(new NDArray<T>(*targets));
            *targetTensor = (*targetVector * *targetTensor) + T(1.0);
            input->applyTriplewiseLambda(targets, targetTensor.get(), mainRoutine, output);
        }

        return ND4J_STATUS_OK;
    }
}
}