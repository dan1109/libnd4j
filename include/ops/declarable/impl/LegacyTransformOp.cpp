//
// Created by raver119 on 16.10.2017.
//

#include <ops/declarable/LegacyTransformOp.h>

#include <NativeOpExcutioner.h>


namespace nd4j {
    namespace ops {
        template <typename T>
        LegacyTransformOp<T>::LegacyTransformOp() : LegacyOp<T>::LegacyOp(1) {
            // just a no-op
        }

        template <typename T>
        LegacyTransformOp<T>::LegacyTransformOp(int opNum) : LegacyOp<T>::LegacyOp(1, opNum) {
            // just a no-op
        }

        template <typename T>
        LegacyOp<T>* LegacyTransformOp<T>::clone() {
            return new LegacyTransformOp(this->_opNum);
        }

        template <typename T>
        Nd4jStatus LegacyTransformOp<T>::validateAndExecute(Context<T> &block) {
            auto input = INPUT_VARIABLE(0);
            auto z = OUTPUT_VARIABLE(0);

            int opNum = block.opNum() < 0 ? this->_opNum : block.opNum();

            NativeOpExcutioner<T>::execTransform(opNum, input->getBuffer(), input->getShapeInfo(), z->getBuffer(), z->getShapeInfo(), block.getTArguments()->data(), nullptr, nullptr);

            STORE_RESULT(*z);

            return ND4J_STATUS_OK;
        }

        /**
        * For transform operations, output shape always equals to input shape. With just a few exclusions, like im2col and col2im. 
        * But these ops already have CustomOp implementations.
        *
        */
        template <typename T>
        ShapeList *LegacyTransformOp<T>::calculateOutputShape(ShapeList *inputShape, nd4j::graph::Context<T> &block) {
            auto inShape = inputShape->at(0);

            int *newShape;
            //ALLOCATE(newShape, ctx.getWorkspace(), shape::shapeInfoLength(inShape), int);
            //memcpy(newShape, inShape, shape::shapeInfoByteLength(inShape));
            COPY_SHAPE(inShape, newShape);

            return SHAPELIST(newShape);
        }


        template class ND4J_EXPORT LegacyTransformOp<float>;
        template class ND4J_EXPORT LegacyTransformOp<double>;
        template class ND4J_EXPORT LegacyTransformOp<float16>;
    }
}