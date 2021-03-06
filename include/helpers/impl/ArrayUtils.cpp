//
// @author raver119@gmail.com
//

#include <helpers/ArrayUtils.h>

namespace nd4j {
    namespace ArrayUtils {
        void toIntPtr(std::initializer_list<int> list, int* target) {
            std::vector<int> vec(list);
            toIntPtr(vec, target);
        }

        void toIntPtr(std::vector<int>& list, int* target) {
            memcpy(target, list.data(), list.size() * sizeof(int));
        }
    }
}
