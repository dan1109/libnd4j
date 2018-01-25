//
//  @author raver119@gmail.com
//

//#include <ops/declarable/headers/parity_ops.h>
#include <ops/declarable/CustomOperations.h>

namespace nd4j {
    namespace ops {
        CUSTOM_OP_IMPL(top_k, 1, 2, false, 0, -2) {
            NDArray<T>* x = INPUT_VARIABLE(0);
            int k = 1;// from params
            bool needSort = true;
            NDArray<T>* values = OUTPUT_VARIABLE(0);
            NDArray<T>* indeces = OUTPUT_VARIABLE(1);
            if (block.numI() > 0) {
                k = INT_ARG(0);
                needSort = INT_ARG(1);
            }

            REQUIRE_TRUE(k <= x->sizeAt(-1), 0, "k should not be greater than last dimension");

            if (k == 1) {
                // using arg_max for it
                //nd4j::ops::argmax<T> op;
                //auto res = op.execute({x}, {}, {x->sizeAt(-1)});

                //REQUIRE_TRUE(res->status() == ND4J_STATUS_OK, 0, "Argmax for top_k failed");
                int width = x->sizeAt(-1);
                int pos = 0;
                for (int e = 0; e < x->lengthOf(); e += width)
                {
                    T topVal = 0;
                    int topIndex = 0;
                    for (int j = 0; j < width; j++) {
                        if (topVal < x->getScalar(j + e))
                        {
                            topVal = x->getScalar(j + e);
                            topIndex = j;
                        }
                    }
                    values->putScalar(pos, topVal);
                    indeces->putScalar(pos++, topIndex);
                }
                //int index = indeces->getScalar(0);
                //T val = x->getScalar(index);
                
                //values->putScalar(0, val);

                return ND4J_STATUS_OK;
            }
            else if (k > 1) {
/*                std::vector<int> inds(k);
                std::vector<T> vals(k);
                std::vector<T> sortedVals;
                for (int e = 0; e < k; e++) {
                    vals[e] = x->getScalar(e); // start initializing
                    inds[e] = e;
                }
                sortedVals = vals;
                std::sort(sortedVals.begin(), sortedVals.end());
                for (int e = k; e < x->lengthOf(); e++) {
                    T v = x->getScalar(e);
                    if (v > sortedVals[0]) { // if the value need to be substituted
                        if (std::find(sortedVals.begin(), sortedVals.end(), v) == sortedVals.end()) {
                        // only for unique values
                            ssize_t ind = std::find(vals.begin(), vals.end(), sortedVals[0]) - vals.begin();
                            vals[ind] = v;
                            inds[ind] = e;
                            sortedVals = vals;
                            std::sort(sortedVals.begin(), sortedVals.end());
                        }
                    }
                }
            
                // if need to be sort results
                if (needSort) {
//                    std::sort(inds.begin(), inds.end(), [vals](int a, int b) {
//                        if (a < vals.size() && b < vals.size())
///                            return vals[a] > vals[b];
//                        else
//                            return true;
//                    });
//
//                    std::sort(vals.begin(), vals.end(), [](int a, int b) {
//                        return a > b;   
//                    });
                }

                for (int e = 0; e < k; e++) {
                    values->putScalar(e, vals.at(e));
                    indeces->putScalar(e, inds.at(e));
                }*/
                return ND4J_STATUS_OK;
            }
            else
                return ND4J_STATUS_BAD_ARGUMENTS;
        }

        DECLARE_SHAPE_FN(top_k) {
            auto shapeList = new ShapeList(); 
            auto in = inputShape->at(0);
            int shapeRank = shape::rank(in);
            int k = 1; // default output shape is size 1

            if (block.numI() > 0) {
                k = INT_ARG(0);
            }

            //REQUIRE_TRUE(k <= x->sizeAt(-1), 0, "k should not be greater than last dimension");

            for (int e = 0; e < 2; e++) { // 2 element tuple at output
                int* newshape;
                ALLOCATE(newshape, block.getWorkspace(), shape::shapeInfoLength(shapeRank), int);
                std::vector<int> internalShape(shapeRank);
                for (int e = 0 ; e < shapeRank - 1; ++e)
                    internalShape[e] = shape::sizeAt(in, e);
                internalShape[shapeRank - 1] = k;
                shape::shapeBuffer(shapeRank, internalShape.data(),  newshape);
                shapeList->push_back(newshape); 
            }
            return shapeList;
        }

    }
}