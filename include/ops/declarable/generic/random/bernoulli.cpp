//
//  @author raver119@gmail.com
//

#include <ops/declarable/headers/random.h>

namespace nd4j {
    namespace ops {
        CUSTOM_OP_IMPL(random_bernoulli, 1, 1, true, 0, 1) {
            auto rng = block.getRNG();

            if (rng == nullptr)
                return Status::THROW("RNG is null, aborting...");

            auto x = INPUT_VARIABLE(0);
            auto z = OUTPUT_VARIABLE(0);

            auto f = (T) INT_ARG(0);

            functions::random::RandomFunction<T>::template execTransform<randomOps::BernoulliDistribution<T>>(block.getRNG(), z->getBuffer(), z->getShapeInfo(), &f);

            return Status::OK();
        }

        DECLARE_SHAPE_FN(random_bernoulli) {
            auto in = INPUT_VARIABLE(0);
            auto shape = in->template asVectorT<int>();

            int *newShape;
            ALLOCATE(newShape, block.getWorkspace(), shape::shapeInfoLength(shape.size()), int);
            shape::shapeBuffer(shape.size(), shape.data(), newShape);

            return SHAPELIST(newShape);
        }
    }
}