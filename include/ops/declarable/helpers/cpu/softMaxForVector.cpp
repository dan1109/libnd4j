//
// Created by Yurii Shyrma on 31.01.2018
//

#include<ops/declarable/helpers/softMaxForVector.h>

namespace nd4j {
namespace ops {
namespace helpers {


///////////////////////////////////////////////////////////////////
template <typename T>
void softMaxForVector(const NDArray<T>& input, NDArray<T>& output) {

	if(!input.isVector() || !output.isVector())
		throw "ops::helpers::softMaxForVector function input and output arrays must be vectors !";

	T* inBuff  = const_cast<NDArray<T>&>(input).getBuffer();
	T* outBuff = output.getBuffer();
	int* inShapeInfo  = input.getShapeInfo();
	int* outShapeInfo = output.getShapeInfo();

	T max = -FLOAT_MAX_VALUE;
	T sum = 0.;
	int inEWS = shape::elementWiseStride(inShapeInfo);
	int outEWS = shape::elementWiseStride(outShapeInfo);
	int length = shape::length(inShapeInfo);
	
	if (inEWS >= 1 && outEWS >= 1) {

		if (inEWS == 1 && outEWS == 1) {

#pragma omp simd reduction(maxT:max)
			for (int i = 0; i < length; i++)
				max = nd4j::math::nd4j_max<T>(max, inBuff[i]);						

#pragma omp parallel for simd reduction(sumT:sum)
			for (int i = 0; i < length; i++) {
        		outBuff[i] = nd4j::math::nd4j_exp<T>(inBuff[i] - max);
				sum += outBuff[i];
			}
#pragma omp simd
			for (int i = 0; i < length; i++)
				outBuff[i] /= sum;
		}
		else {

#pragma omp simd reduction(maxT:max)
			for (int i = 0; i < length; i++)
				max = nd4j::math::nd4j_max<T>(max, inBuff[i * inEWS]);

#pragma omp parallel for simd reduction(sumT:sum)
			for (int i = 0; i < length; i++) {
                T r = nd4j::math::nd4j_exp<T>(inBuff[i * inEWS] - max);
                outBuff[i * outEWS] = r;
				sum += r;
			}
#pragma omp simd
			for (int i = 0; i < length; i++)
				outBuff[i * outEWS] /= sum;						
		}
	}
}


///////////////////////////////////////////////////////////////////
template <typename T>
void logSoftMaxForVector(const NDArray<T>& input, NDArray<T>& output) {

	if(!input.isVector() || !output.isVector())
		throw "ops::helpers::logSoftMaxForVector function input and output arrays must be vectors !";

	T* inBuff  = const_cast<NDArray<T>&>(input).getBuffer();
	T* outBuff = output.getBuffer();
	int* inShapeInfo  = input.getShapeInfo();
	int* outShapeInfo = output.getShapeInfo();

	T max = -FLOAT_MAX_VALUE;
	T sum = 0;

	int inEWS  = shape::elementWiseStride(inShapeInfo);
	int length = shape::length(inShapeInfo);
	
	if (inEWS == 1) {
#pragma omp simd reduction(maxT:max)
		for (int i = 0; i < length; i++)
			max = nd4j::math::nd4j_max<T>(max, outBuff[i]);

#pragma omp simd reduction(sumT:sum)
		for (int i = 0; i < length; i++) {
			outBuff[i] = nd4j::math::nd4j_exp<T>(inBuff[i] - max);
			sum += outBuff[i];
		}
#pragma omp simd
		for (int i = 0; i < length; i++) {
			outBuff[i] /= sum;
			outBuff[i] = nd4j::math::nd4j_log<T>(outBuff[i]);
		}
	}
	else if (inEWS > 1) {

#pragma omp simd reduction(maxT:max)
		for (int i = 0; i < length; i++)
			max = nd4j::math::nd4j_max<T>(max, outBuff[i * inEWS]);

#pragma omp simd reduction(sumT:sum)
		for (int i = 0; i < length; i++) {
			outBuff[i * inEWS] = nd4j::math::nd4j_exp<T>(inBuff[i * inEWS] - max);
			sum += outBuff[i * inEWS];
		}
#pragma omp simd
		for (int i = 0; i < length; i++) {
			outBuff[i * inEWS] /= sum;
			outBuff[i * inEWS] = nd4j::math::nd4j_log<T>(outBuff[i * inEWS]);
		}
	}	
}


template void softMaxForVector<float>  (const NDArray<float  >& input, NDArray<float  >& output);
template void softMaxForVector<float16>(const NDArray<float16>& input, NDArray<float16>& output);
template void softMaxForVector<double> (const NDArray<double >& input, NDArray<double >& output);

template void logSoftMaxForVector<float>  (const NDArray<float  >& input, NDArray<float  >& output);
template void logSoftMaxForVector<float16>(const NDArray<float16>& input, NDArray<float16>& output);
template void logSoftMaxForVector<double> (const NDArray<double >& input, NDArray<double >& output);


}
}
}

