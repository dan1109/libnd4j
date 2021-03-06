//
// @author @cpuheater
//

#include <ops/declarable/CustomOperations.h>
#include <helpers/ShapeUtils.h>
#include <NDArray.h>
#include <array/NDArrayList.h>
#include <array>

namespace nd4j {
    namespace ops {
        CUSTOM_OP_IMPL(confusion_matrix, 2, 1, false, 0, -2) {

            auto labels = INPUT_VARIABLE(0);
            auto predictions = INPUT_VARIABLE(1);
            NDArray<T>* weights = nullptr;
            if(block.width() > 2){
                weights = INPUT_VARIABLE(2);
                REQUIRE_TRUE(weights->isSameShape(predictions),0, "CONFUSION_MATRIX: Weights and predictions should have equal shape");
            }
            auto output = OUTPUT_VARIABLE(0);

            int minPrediction = predictions->template reduceNumber<simdOps::Min<T>>();
            int minLabel = labels->template reduceNumber<simdOps::Min<T>>();

            REQUIRE_TRUE(minLabel >=0, 0, "CONFUSION_MATRIX: Labels contains negative values !");
            REQUIRE_TRUE(minPrediction >=0, 0, "CONFUSION_MATRIX: Predictions contains negative values !");
            REQUIRE_TRUE(labels->isVector(), 0, "CONFUSION_MATRIX: Labels input should be a Vector, but got %iD instead", labels->rankOf());
            REQUIRE_TRUE(predictions->isVector(), 0, "CONFUSION_MATRIX: Predictions input should be Vector, but got %iD instead", predictions->rankOf());
            REQUIRE_TRUE(labels->isSameShape(predictions),0, "CONFUSION_MATRIX: Labels and predictions should have equal shape");

            ResultSet<T>* arrs = NDArrayFactory<T>::allTensorsAlongDimension(output, {1});


            for (int j = 0; j < labels->lengthOf(); ++j){
                Nd4jIndex label = (*labels)(j);
                Nd4jIndex pred = (*predictions)(j);
                T value = (weights==nullptr) ? (T)1.0 : (*weights)(j);
                (*arrs->at(label))(pred) = value;
            }

            delete arrs;

            return ND4J_STATUS_OK;
        }

        DECLARE_SHAPE_FN(confusion_matrix) {

            auto labels = INPUT_VARIABLE(0);
            auto predictions = INPUT_VARIABLE(1);

            int numClasses = 0;

            if (block.getIArguments()->size() > 0) {
                numClasses = INT_ARG(0);
            }
            else  {
                int maxPrediction = predictions->template reduceNumber<simdOps::Max<T>>();
                int maxLabel = labels->template reduceNumber<simdOps::Max<T>>();
                numClasses = (maxPrediction >= maxLabel) ?  maxPrediction+1 : maxLabel+1;
            }

            int *newShape;
            std::array<int, 2> shape = {{numClasses,numClasses}};
            ALLOCATE(newShape, block.getWorkspace(), shape::shapeInfoLength(2), int);
            shape::shapeBuffer(2, shape.data(), newShape);

            return SHAPELIST(newShape);
        }
    }
}
