//
// Created by GS <sgazeos@gmail.com> 31.01.2018
//

#include <ops/declarable/CustomOperations.h>

namespace nd4j {
    namespace ops {
        CUSTOM_OP_IMPL(zero_fraction, 1, 1, false, 0, 0) {
            auto input = INPUT_VARIABLE(0);
            auto output = OUTPUT_VARIABLE(0);

            REQUIRE_TRUE(output->isScalar(), 0, "Rank output should be scalar");
            int numZeros = 0;
//            for (int e = 0; e < input->lengthOf(); e++)
//                if ((*input)(e) == T(0))
//                    numZeros++;
            T fraction = input->template reduceNumber<simdOps::CountZero<T>>();//((T)(0) + numZeros) / input->lengthOf();
            fraction /= input->lengthOf();
            output->putScalar(0, fraction);

            return ND4J_STATUS_OK;
        }
        DECLARE_SHAPE_FN(zero_fraction) {
            int *newShape;
            ALLOCATE(newShape, block.getWorkspace(), shape::shapeInfoLength(0), int);

            shape::shapeScalar(newShape);

            return SHAPELIST(newShape);
        }
    }
}