//
// Created by raver119 on 16.10.2017.
//

#include <ops/declarable/LegacyScalarOp.h>


namespace nd4j {
    namespace ops {
        template <typename T>
        LegacyScalarOp<T>::LegacyScalarOp() : LegacyOp<T>::LegacyOp(1) {
            // no-op
        }

        template <typename T>
        LegacyScalarOp<T>::LegacyScalarOp(int opNum)  : LegacyOp<T>::LegacyOp(1, opNum){
            // no-op
        }

        template <typename T>
        LegacyOp<T>* LegacyScalarOp<T>::clone() {
            return new LegacyScalarOp(this->_opNum, this->_scalar);
        }

        template <typename T>
        LegacyScalarOp<T>::LegacyScalarOp(int opNum, T scalar)  : LegacyOp<T>::LegacyOp(1, opNum){
            _scalar = scalar;
        }

        template <typename T>
        ShapeList *LegacyScalarOp<T>::calculateOutputShape(ShapeList *inputShape, nd4j::graph::Context<T> &block) {
            auto inShape = inputShape->at(0);

            int *newShape;
            ALLOCATE(newShape, block.getWorkspace(), shape::shapeInfoLength(inShape), int);
            memcpy(newShape, inShape, shape::shapeInfoByteLength(inShape));

            return SHAPELIST(newShape);
        }


        template <typename T>
        Nd4jStatus LegacyScalarOp<T>::validateAndExecute(Context<T> &block) {
            auto x = INPUT_VARIABLE(0);
            T scalar = (T) 0.0f;
            int offset = 0;
            if (block.width() > 1) {
                auto y = INPUT_VARIABLE(1);
                scalar = y->getScalar(0);
            } else if (block.getTArguments()->size() > 0) {
                scalar = T_ARG(0);
                offset++;
            } else {
                scalar = _scalar;
            }

            auto z = OUTPUT_VARIABLE(0);

            int opNum = block.opNum() < 0 ? this->_opNum : block.opNum();

            NativeOpExcutioner<T>::execScalar(opNum, x->getBuffer(), x->getShapeInfo(), z->getBuffer(), z->getShapeInfo(), scalar, block.getTArguments()->data() + offset);

            STORE_RESULT(*z);

            return ND4J_STATUS_OK;
        }

        template class ND4J_EXPORT LegacyScalarOp<float>;
        template class ND4J_EXPORT LegacyScalarOp<float16>;
        template class ND4J_EXPORT LegacyScalarOp<double>;
    }
}